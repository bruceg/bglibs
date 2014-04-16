#include <string.h>

#include "dns.h"

static int getit(struct dns_result* out, unsigned int i,
		 const char* buf, unsigned int len, unsigned int pos, uint16 datalen)
{
  unsigned char pref[2];
  char* q = 0;
  struct dns_mx* mx = &out->rr.mx[i];

  if (!dns_packet_copy(buf,len,pos,pref,2)) return -1;
  if (!dns_packet_getname(buf,len,pos + 2,&q)) return -1;
  mx->distance = uint16_get_msb(pref);
  len = fmt_dns_domain(0, q);
  if ((mx->name = malloc(len + 1)) == 0) { free(q); return -1; }
  fmt_dns_domain(mx->name, q);
  mx->name[len] = 0;
  free(q);
  return 0;
  (void)datalen;
}

/** Extract mail exchanger (MX) records from a DNS response packet. */
int dns_mx_packet(struct dns_result* out, const char* buf, unsigned int len)
{
  return dns_packet_extract(out, buf, len, DNS_T_MX, DNS_C_IN, getit);
}

/** Request the mail exchanger (MX) records for a domain name. */
int dns_mx_r(struct dns_transmit* tx, struct dns_result* out, const char* fqdn)
{
  char *q = 0;
  if (!dns_domain_fromdot(&q,fqdn,strlen(fqdn))) return -1;
  if (dns_resolve(tx,q,DNS_T_MX) == -1) { free(q); return -1; }
  free(q);
  if (dns_mx_packet(out,tx->packet,tx->packetlen) == -1)  return -1;
  dns_transmit_free(tx);
  return 0;
}

/** \fn dns_mx(struct dns_result*, const char*)
    Request the mail exchanger (MX) records for a domain name.
*/
DNS_R_FN_WRAP(mx, const char*)

#ifdef SELFTEST_MAIN
#include "str/iter.h"
MAIN
{
  struct dns_result out = {0};
  int i;

  debugfn(dns_mx(&out, "untroubled.org"));
  for (i = 0; i < out.count; ++i)
    obuf_putf(&outbuf, "d{ }d{: \"}s{\"\n}", i, out.rr.mx[i].distance, out.rr.mx[i].name);
}
#endif
#ifdef SELFTEST_EXP
result=0
0 12801: "mx.futurequest.net"
#endif
