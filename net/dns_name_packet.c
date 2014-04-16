#include <string.h>

#include "dns.h"

static int sizeit(const char* buf, unsigned int len, unsigned int pos, uint16 datalen)
{
  char* q = 0;
  if (!dns_packet_getname(buf,len,pos,&q)) return -1;
  len = fmt_dns_domain(0, q);
  free(q);
  return len + 1;
  (void)datalen;
}

static int getit(struct dns_result* out, unsigned int i, unsigned int offset,
		 const char* buf, unsigned int len, unsigned int pos, uint16 datalen)
{
  char* q = 0;
  char** name = &out->rr.name[i];

  if (!dns_packet_getname(buf,len,pos,&q)) return -1;
  *name = out->__buffer + offset;
  len = fmt_dns_domain(*name, q);
  (*name)[len] = 0;
  free(q);
  return len + 1;
  (void)datalen;
}

/** Extract name (PTR) records from a DNS response packet. */
int dns_name_packet(struct dns_result* out, const char* buf, unsigned int len)
{
  return dns_packet_extract(out, buf, len, DNS_T_PTR, DNS_C_IN, sizeit, getit);
}
