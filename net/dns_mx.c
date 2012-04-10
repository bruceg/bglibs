#include <string.h>

#include "dns.h"

static int getit(str* out, const char* buf, unsigned int len, unsigned int pos, uint16 datalen)
{
  unsigned char pref[2];
  char* q = 0;
  if (!dns_packet_copy(buf,len,pos,pref,2)) return -1;
  if (!dns_packet_getname(buf,len,pos + 2,&q)) return -1;
  if (!str_catb(out,(char*)pref,2)) { free(q); return -1; }
  if (!dns_domain_todot_cat(out,q)) { free(q); return -1; }
  free(q);
  if (!str_catc(out,0)) return -1;
  return 1;
  (void)datalen;
}

/** Extract mail exchanger (MX) records from a DNS response packet. */
int dns_mx_packet(str *out,const char *buf,unsigned int len)
{
  return dns_packet_extract(out, buf, len, DNS_T_MX, DNS_C_IN, getit);
}

/** Request the mail exchanger (MX) records for a domain name. */
int dns_mx_r(struct dns_transmit *tx,str *out,const char *fqdn)
{
  char *q = 0;
  if (!dns_domain_fromdot(&q,fqdn,strlen(fqdn))) return -1;
  if (dns_resolve(tx,q,DNS_T_MX) == -1) { free(q); return -1; }
  free(q);
  if (dns_mx_packet(out,tx->packet,tx->packetlen) == -1)  return -1;
  dns_transmit_free(tx);
  return 0;
}

/** \fn dns_mx(str*, const char*)
    Request the mail exchanger (MX) records for a domain name.
*/
DNS_R_FN_WRAP2(dns_mx, str*, const char*)

#ifdef SELFTEST_MAIN
#include "striter.h"
MAIN
{
  str out = {0};
  striter i;

  dns_mx(&out, "untroubled.org");
  striter_loop(&i, &out, 0) {
    obuf_putf(&outbuf, "d{ }d{: \"}s{\"\n}",
	      uint16_get_msb((unsigned char*)i.startptr),
	      i.len - 2,
	      i.startptr + 2);
  }
  obuf_flush(&outbuf);
}
#endif
#ifdef SELFTEST_EXP
12801 18: "mx.futurequest.net"
#endif
