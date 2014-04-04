#include <string.h>

#include "dns.h"

static void iopause(iopoll_fd *x,unsigned int len,struct timeval *deadline,struct timeval *stamp)
{
  int millisecs;
  unsigned int i;

  if (TV_LESS(deadline,stamp))
    millisecs = 0;
  else {
    if (deadline->tv_sec - stamp->tv_sec > 1000)
      millisecs = 1000000;
    else
      millisecs = (deadline->tv_sec - stamp->tv_sec) * 1000 + (deadline->tv_usec - stamp->tv_usec) / 1000;
    millisecs += 20;
  }

  for (i = 0;i < len;++i)
    x[i].revents = 0;

  iopoll(x,len,millisecs);
}

int dns_resolve(struct dns_transmit *tx,const char *q,uint16 qtype)
{
  struct timeval stamp;
  struct timeval deadline;
  ipv4addr servers[DNS_MAX_IPS];
  ipv4addr ipzero;
  iopoll_fd x[1];
  int r;

  if (dns_resolvconfip(servers) == -1) return -1;
  memset(&ipzero,0,sizeof ipzero);
  if (dns_transmit_start(tx,servers,1,q,qtype,&ipzero) == -1) return -1;

  for (;;) {
    gettimeofday(&stamp,0);
    deadline = stamp;
    deadline.tv_sec += 120;
    dns_transmit_io(tx,x,&deadline);
    iopause(x,1,&deadline,&stamp);
    r = dns_transmit_get(tx,x,&stamp);
    if (r == -1) return -1;
    if (r == 1) return 0;
  }
}
