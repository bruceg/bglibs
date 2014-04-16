#include <errno.h>
#include <string.h>

#include "dns.h"

static int getit(struct dns_result* out, unsigned int i,
		 const char* buf, unsigned int len, unsigned int pos, uint16 datalen)
{
  unsigned char header[4];
  if (datalen != 4) { errno = EPROTO; return -1; }
  if (!dns_packet_copy(buf,len,pos,header,4)) { errno = EPROTO; return -1; }
  memcpy(&out->rr.ip4[i], header, 4);
  return 0;
}

/** Extract IPv4 address (A) records from a DNS response packet. */
int dns_ip4_packet(struct dns_result* out, const char* buf, unsigned int len)
{
  if (dns_packet_extract(out, buf, len, DNS_T_A, DNS_C_IN, getit) < 0)
    return -1;
  dns_rotateipv4(out->rr.ip4, out->count);
  return 0;
}

/** Request the IPv4 address (A) records for a domain name. */
int dns_ip4_r(struct dns_transmit* tx, struct dns_result *out, const char* fqdn)
{
  char *q = 0;
  ipv4addr ip;
  const char* x;

  if ((x = ipv4_scan(fqdn, &ip)) != 0 && *x == 0) {
    if (dns_result_alloc(out, DNS_T_A, 1) < 0)
      return -1;
    memcpy(out->rr.ip4, &ip, sizeof ip);
    return 0;
  }

  if (!dns_domain_fromdot(&q,fqdn,strlen(fqdn))) return -1;
  if (dns_resolve(tx,q,DNS_T_A) == -1) { free(q); return -1; }
  free(q);
  if (dns_ip4_packet(out,tx->packet,tx->packetlen) == -1)  return -1;
  dns_transmit_free(tx);
  return 0;
}

/** \fn dns_ip4(struct dns_result*, const char*)
    Request the IPv4 address (A) records for a domain name.
*/
DNS_R_FN_WRAP(ip4, const char*)

#ifdef SELFTEST_MAIN
struct dns_result out = {0};
void doit(const char* fqdn)
{
  int i;

  debugfn(dns_ip4(&out, fqdn));
  obuf_putf(&outbuf, "s{ }d{:}", fqdn, out.count);
  for (i = 0; i < out.count; i++) {
    obuf_putc(&outbuf, ' ');
    obuf_puts(&outbuf, ipv4_format(&out.rr.ip4[i]));
  }
  NL();
}
MAIN
{
  doit("1.2.3.4");
  doit("untroubled.org");
}
#endif
#ifdef SELFTEST_EXP
result=0
1.2.3.4 1: 1.2.3.4
result=0
untroubled.org 1: 69.5.1.51
#endif
