#include <sysdeps.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "socket.h"
#include "dns.h"

static int serverwantstcp(const char *buf,unsigned int len)
{
  unsigned char out[12];

  if (!dns_packet_copy(buf,len,0,out,12)) return 1;
  if (out[2] & 2) return 1;
  return 0;
}

static int serverfailed(const char *buf,unsigned int len)
{
  unsigned char out[12];
  unsigned int rcode;

  if (!dns_packet_copy(buf,len,0,out,12)) return 1;
  rcode = out[3];
  rcode &= 15;
  if (rcode && (rcode != 3)) { errno = EAGAIN; return 1; }
  return 0;
}

static int irrelevant(const struct dns_transmit *d,const char *buf,unsigned int len)
{
  unsigned char out[12];
  char *dn;
  unsigned int pos;

  pos = dns_packet_copy(buf,len,0,out,12); if (!pos) return 1;
  if (memcmp(out,d->query + 2,2)) return 1;
  if (out[4] != 0) return 1;
  if (out[5] != 1) return 1;

  dn = 0;
  pos = dns_packet_getname(buf,len,pos,&dn); if (!pos) return 1;
  if (!dns_domain_equal(dn,d->query + 14)) { free(dn); return 1; }
  free(dn);

  pos = dns_packet_copy(buf,len,pos,out,4); if (!pos) return 1;
  if (uint16_get_msb(out) != d->qtype) return 1;
  if (uint16_get_msb(out + 2) != DNS_C_IN) return 1;

  return 0;
}

static void packetfree(struct dns_transmit *d)
{
  if (!d->packet) return;
  free(d->packet);
  d->packet = 0;
}

static void queryfree(struct dns_transmit *d)
{
  if (!d->query) return;
  free(d->query);
  d->query = 0;
}

static void socketfree(struct dns_transmit *d)
{
  if (!d->s1) return;
  close(d->s1 - 1);
  d->s1 = 0;
}

/** Free all allocated resources in a \c dns_transmit structure. */
void dns_transmit_free(struct dns_transmit *d)
{
  queryfree(d);
  socketfree(d);
  packetfree(d);
}

static int randombind(struct dns_transmit *d)
{
  int j;

  for (j = 0;j < 10;++j)
    if (socket_bind4(d->s1 - 1,&d->localip,1025 + dns_random(64510)))
      return 0;
  if (socket_bind4(d->s1 - 1,&d->localip,0))
    return 0;
  return -1;
}

static const int timeouts[4] = { 1, 3, 11, 45 };

static int thisudp(struct dns_transmit *d)
{
  const ipv4addr *ip;

  socketfree(d);

  while (d->udploop < 4) {
    for (;d->curserver < DNS_MAX_IPS;++d->curserver) {
      ip = d->servers + d->curserver;
      if (memcmp(ip,"\0\0\0\0",4)) {
	d->query[2] = dns_random(256);
	d->query[3] = dns_random(256);

        d->s1 = 1 + socket_udp();
        if (!d->s1) { dns_transmit_free(d); return -1; }
	if (randombind(d) == -1) { dns_transmit_free(d); return -1; }

        if (socket_connect4(d->s1 - 1,ip,53))
          if (send(d->s1 - 1,d->query + 2,d->querylen - 2,0) == d->querylen - 2) {
	    gettimeofday(&d->deadline,0);
	    d->deadline.tv_sec += timeouts[d->udploop];
            d->tcpstate = 0;
            return 0;
          }

        socketfree(d);
      }
    }

    ++d->udploop;
    d->curserver = 0;
  }

  dns_transmit_free(d); return -1;
}

static int firstudp(struct dns_transmit *d)
{
  d->curserver = 0;
  return thisudp(d);
}

static int nextudp(struct dns_transmit *d)
{
  ++d->curserver;
  return thisudp(d);
}

static int thistcp(struct dns_transmit *d)
{
  const ipv4addr *ip;

  socketfree(d);
  packetfree(d);

  for (;d->curserver < DNS_MAX_IPS;++d->curserver) {
    ip = d->servers + d->curserver;
    if (memcmp(ip,"\0\0\0\0",4)) {
      d->query[2] = dns_random(256);
      d->query[3] = dns_random(256);

      d->s1 = 1 + socket_tcp();
      if (!d->s1) { dns_transmit_free(d); return -1; }
      if (randombind(d) == -1) { dns_transmit_free(d); return -1; }

      gettimeofday(&d->deadline,0);
      d->deadline.tv_sec += 10;
      if (socket_connect4(d->s1 - 1,ip,53)) {
        d->tcpstate = 2;
        return 0;
      }
      if ((errno == EINPROGRESS) || (errno == EWOULDBLOCK)) {
        d->tcpstate = 1;
        return 0;
      }

      socketfree(d);
    }
  }

  dns_transmit_free(d); return -1;
}

static int firsttcp(struct dns_transmit *d)
{
  d->curserver = 0;
  return thistcp(d);
}

static int nexttcp(struct dns_transmit *d)
{
  ++d->curserver;
  return thistcp(d);
}

/** Start the transmission of a DNS query.

    \param d The record of the transmission state.
    \param servers The list of servers to contact, typically filled by \c dns_resolvconfip.
    \param flagrecursive Use recursive queries (ie querying a cache).
    \param q The domain name to query, in DNS format.
    \param qtype The query type number.
    \param localip The local IP from which to send the query, may be \c NULL.
 */
int dns_transmit_start(struct dns_transmit *d,const ipv4addr servers[DNS_MAX_IPS],int flagrecursive,const char *q,uint16 qtype,const ipv4addr *localip)
{
  unsigned int len;

  dns_transmit_free(d);
  errno = EIO;

  len = dns_domain_length(q);
  d->querylen = len + 18;
  d->query = malloc(d->querylen);
  if (!d->query) return -1;

  uint16_pack_msb(len+16,(unsigned char*)d->query);
  memcpy(d->query + 2,flagrecursive ? "\0\0\1\0\0\1\0\0\0\0\0\0" : "\0\0\0\0\0\1\0\0\0\0\0\0gcc-bug-workaround",12);
  memcpy(d->query + 14,q,len);
  uint16_pack_msb(qtype,(unsigned char*)d->query + 14 + len);
  uint16_pack_msb(DNS_C_IN,(unsigned char*)d->query + 16 + len);

  d->qtype = qtype;
  d->servers = servers;
  if (localip != 0)
    d->localip = *localip;
  else
    memset(&d->localip, 0, sizeof d->localip);

  d->udploop = flagrecursive ? 1 : 0;

  if (len + 16 > 512) return firsttcp(d);
  return firstudp(d);
}

/** Fill in the \c iopoll_fd structure from a \c dns_transmit structure. */
void dns_transmit_io(const struct dns_transmit *d,iopoll_fd *x,struct timeval *deadline)
{
  x->fd = d->s1 - 1;

  switch(d->tcpstate) {
    case 0: case 3: case 4: case 5:
      x->events = IOPOLL_READ;
      break;
    case 1: case 2:
      x->events = IOPOLL_WRITE;
      break;
  }

  if (TV_LESS(&d->deadline,deadline))
    *deadline = d->deadline;
}

/** Complete I/O on a DNS query.

    \returns 1 when the query is completed.
*/
int dns_transmit_get(struct dns_transmit *d,const iopoll_fd *x,const struct timeval *when)
{
  char udpbuf[513];
  unsigned char ch;
  int r;
  int fd;

  errno = EIO;
  fd = d->s1 - 1;

  if (!x->revents) {
    if (TV_LESS(when,&d->deadline)) return 0;
    errno = ETIMEDOUT;
    if (d->tcpstate == 0) return nextudp(d);
    return nexttcp(d);
  }

  if (d->tcpstate == 0) {
/*
have attempted to send UDP query to each server udploop times
have sent query to curserver on UDP socket s
*/
    r = recv(fd,udpbuf,sizeof udpbuf,0);
    if (r <= 0) {
      if (errno == ECONNREFUSED) if (d->udploop == 2) return 0;
      return nextudp(d);
    }
    if (r + 1 > (int)sizeof udpbuf) return 0;

    if (irrelevant(d,udpbuf,r)) return 0;
    if (serverwantstcp(udpbuf,r)) return firsttcp(d);
    if (serverfailed(udpbuf,r)) {
      if (d->udploop == 2) return 0;
      return nextudp(d);
    }
    socketfree(d);

    d->packetlen = r;
    d->packet = malloc(d->packetlen);
    if (!d->packet) { dns_transmit_free(d); return -1; }
    memcpy(d->packet,udpbuf,d->packetlen);
    queryfree(d);
    return 1;
  }

  if (d->tcpstate == 1) {
/*
have sent connection attempt to curserver on TCP socket s
pos not defined
*/
    if (!socket_connected(fd)) return nexttcp(d);
    d->pos = 0;
    d->tcpstate = 2;
    return 0;
  }

  if (d->tcpstate == 2) {
/*
have connection to curserver on TCP socket s
have sent pos bytes of query
*/
    r = write(fd,d->query + d->pos,d->querylen - d->pos);
    if (r <= 0) return nexttcp(d);
    d->pos += r;
    if (d->pos == d->querylen) {
      gettimeofday(&d->deadline,0);
      d->deadline.tv_sec += 10;
      d->tcpstate = 3;
    }
    return 0;
  }

  if (d->tcpstate == 3) {
/*
have sent entire query to curserver on TCP socket s
pos not defined
*/
    r = read(fd,&ch,1);
    if (r <= 0) return nexttcp(d);
    d->packetlen = ch;
    d->tcpstate = 4;
    return 0;
  }

  if (d->tcpstate == 4) {
/*
have sent entire query to curserver on TCP socket s
pos not defined
have received one byte of packet length into packetlen
*/
    r = read(fd,&ch,1);
    if (r <= 0) return nexttcp(d);
    d->packetlen <<= 8;
    d->packetlen += ch;
    d->tcpstate = 5;
    d->pos = 0;
    d->packet = malloc(d->packetlen);
    if (!d->packet) { dns_transmit_free(d); return -1; }
    return 0;
  }

  if (d->tcpstate == 5) {
/*
have sent entire query to curserver on TCP socket s
have received entire packet length into packetlen
packet is allocated
have received pos bytes of packet
*/
    r = read(fd,d->packet + d->pos,d->packetlen - d->pos);
    if (r <= 0) return nexttcp(d);
    d->pos += r;
    if (d->pos < d->packetlen) return 0;

    socketfree(d);
    if (irrelevant(d,d->packet,d->packetlen)) return nexttcp(d);
    if (serverwantstcp(d->packet,d->packetlen)) return nexttcp(d);
    if (serverfailed(d->packet,d->packetlen)) return nexttcp(d);

    queryfree(d);
    return 1;
  }

  return 0;
}
