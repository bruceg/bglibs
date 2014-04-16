#include <string.h>

#include "dns.h"

static int getit(struct dns_result* out, unsigned int i,
		 const char* buf, unsigned int len, unsigned int pos, uint16 datalen)
{
  char* q = 0;
  char** name = &out->rr.name[i];

  if (!dns_packet_getname(buf,len,pos,&q)) return -1;
  len = fmt_dns_domain(0, q);
  if ((*name = malloc(len + 1)) == 0) { free(q); return -1; }
  fmt_dns_domain(*name, q);
  (*name)[len] = 0;
  free(q);
  return 0;
  (void)datalen;
}

/** Extract name (PTR) records from a DNS response packet. */
int dns_name_packet(struct dns_result* out, const char* buf, unsigned int len)
{
  return dns_packet_extract(out, buf, len, DNS_T_PTR, DNS_C_IN, getit);
}
