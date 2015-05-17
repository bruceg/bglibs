#include "fmt.h"
#include "iobuf.h"
#include "ipv4.h"
#include "msg.h"
#include "socket.h"
#include "uint16.h"
#include "uint32.h"

static int dns_responder_pid = 0;
extern ipv4port dns_use_port;
struct dns_response_rr
{
  const char* name;
  uint16 namelen;
  uint16 type;
  uint16 class;
  uint32 ttl;
  const char* rdata;
  uint16 rdatalen;
};
struct dns_response
{
  uint16 ancount, nscount, arcount;
  struct dns_response_rr rr[];
};

static long dump_name(const unsigned char* data)
{
  long o = 0;
  unsigned char l;
  while ((l = data[o++]) > 0) {
    obuf_write(&outbuf, (const char*)data+o, l);
    obuf_putc(&outbuf, '.');
    o += l;
  };
  return o;
}

static long dump_request(const unsigned char* packet, long len)
{
  uint16 first = uint16_get_msb(packet+2);
  uint16 qdcount = uint16_get_msb(packet+4);
  uint16 ancount = uint16_get_msb(packet+6);
  uint16 nscount = uint16_get_msb(packet+8);
  uint16 arcount = uint16_get_msb(packet+10);
  long o = 12;
  obuf_putf(&outbuf, "d{: ID=XX QR=}d{ opcode=}d{ AA=}d{ TC=}d{ RD=}d{ RA=}d{ Z=}d{ RCODE=}d{ QDCOUNT=}d{ ANCOUNT=}d{ NSCOUNT=}d{ ARCOUNT=}d{\n}",
	    len,
	    first >> 15,	/* QR */
	    (first >> 11) & 0xf, /* Opcode */
	    (first >> 10) & 1,	 /* AA */
	    (first >> 9) & 1,	 /* TC */
	    (first >> 8) & 1,	 /* RD */
	    (first >> 7) & 1,	 /* RA */
	    (first >> 4) & 7,	 /* Z */
	    first & 0xf,	 /* RCODE */
	    qdcount, ancount, nscount, arcount);
  for (; qdcount > 0; qdcount--) {
    obuf_puts(&outbuf, "Question: ");
    o += dump_name(packet+o);
    obuf_putf(&outbuf, "{ QTYPE=}d{ QCLASS=}d{\n}", uint16_get_msb(packet+o), uint16_get_msb(packet+o+2));
    o += 4;
  }
  obuf_flush(&outbuf);
  return o;
}

static int make_response(unsigned char* buf, int offset, const struct dns_response* response)
{
  int r;
  uint16_pack_msb(response->ancount, buf+6);
  uint16_pack_msb(response->nscount, buf+8);
  uint16_pack_msb(response->arcount, buf+10);
  for (r = 0; r < response->ancount + response->nscount + response->arcount; r++) {
    const struct dns_response_rr* rr = &response->rr[r];
    memcpy((char*)buf+offset, rr->name, rr->namelen);
    offset += rr->namelen;
    uint16_pack_msb(rr->type, buf+offset);
    uint16_pack_msb(rr->class, buf+offset+2);
    uint32_pack_msb(rr->ttl, buf+offset+4);
    uint16_pack_msb(rr->rdatalen, buf+offset+8);
    offset += 10;
    memcpy((char*)buf+offset, rr->rdata, rr->rdatalen);
    offset += rr->rdatalen;
  }
  return offset;
}

static void start_dns_responder(const struct dns_response* response)
{
  int sockfd;
  ipv4addr addr;
  ipv4port port;
  unsigned char buf[512];
  long r;
  long offset;

  if ((sockfd = socket_udp()) < 0)
    die1sys(111, "Could not create DNS socket");
  ipv4_scan("127.0.0.1", &addr);
  if (!socket_bind4(sockfd, &addr, 0))
    die1sys(111, "Could not bind DNS socket");
  if (!socket_getaddr4(sockfd, &addr, &port))
    die1sys(111, "Could not get DNS socket address");
  if (0 && !socket_listen(sockfd, 0))
    die1sys(111, "Could not listen to DNS socket");
  dns_use_port = port;
  buf[fmt_sdec((char*)buf, port)] = 0;
  if (setenv("DNSCACHEIP", "127.0.0.1", 1) < 0
      || setenv("DNSCACHEPORT", (char*)buf, 1) < 0)
    die1sys(111, "Could not set up DNS responder environment variables");
  if ((dns_responder_pid = fork()) < 0)
    die1sys(111, "Could not fork");
  if (dns_responder_pid != 0)
    return;
  if ((r = socket_recv4(sockfd, (char*)buf, sizeof buf, &addr, &port)) < 0)
    die1sys(111, "Could not receive DNS packet");
  offset = dump_request(buf, r);
  offset = make_response(buf, offset, response);
  if ((r = socket_send4(sockfd, (char*)buf, offset, &addr, port)) != offset)
    die1sys(111, "Could not send DNS response");
  exit(0);
}

void do_dns_test(const char* fqdn,
		 int (*fn)(struct dns_result*, const char*),
		 void (*dump)(int, const union dns_result_rrs* rrs))
{
  struct dns_result out = {0};
  debugfn(fn(&out, fqdn));
  obuf_putf(&outbuf, "s{: count=}d{\n}", fqdn, out.count);
  dump(out.count, &out.rr);
}

void do_dns_respond_test(const char* fqdn,
			 const struct dns_response* response,
			 int (*fn)(struct dns_result*, const char*),
			 void (*dump)(int, const union dns_result_rrs* rrs))
{
  start_dns_responder(response);
  do_dns_test(fqdn, fn, dump);
}
