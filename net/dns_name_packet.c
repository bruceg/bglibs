#include <string.h>

#include "dns.h"

static int getit(str* out, const char* buf, unsigned int len, unsigned int pos, uint16 datalen)
{
  char* q = 0;
  int r;
  if (!dns_packet_getname(buf,len,pos,&q)) return -1;
  r = dns_domain_todot_cat(out,q);
  free(q);
  return r ? 0 : -1;
  (void)datalen;
}

/** Extract name (PTR) records from a DNS response packet. */
int dns_name_packet(str *out,const char *buf,unsigned int len)
{
  return dns_packet_extract(out, buf, len, DNS_T_PTR, DNS_C_IN, getit);
}
