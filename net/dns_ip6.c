#include <errno.h>
#include <string.h>

#include "dns.h"

static int getit(struct dns_result* out, unsigned int i, unsigned int offset,
		 const char* buf, unsigned int len, unsigned int pos, uint16 datalen)
{
  unsigned char header[4];
  if (datalen != 16) { errno = EPROTO; return -1; }
  if (!dns_packet_copy(buf,len,pos,header,16)) return -1;
  memcpy(&out->rr.ip6[i], header, 16);
  return 0;
  (void)offset;
}

/** Extract IPv6 address (AAAA) records from a DNS response packet. */
int dns_ip6_packet(struct dns_result* out, const char* buf, unsigned int len)
{
  if (dns_packet_extract(out, buf, len, DNS_T_AAAA, DNS_C_IN, 0, getit) < 0)
    return -1;
  dns_rotateipv6(out->rr.ip6, out->count);
  return 0;
}

/** Request the IPv6 address (AAAA) records for a domain name. */
int dns_ip6_r(struct dns_transmit* tx, struct dns_result* out, const char* fqdn)
{
  char *q = 0;
  ipv6addr ip6;
  ipv4addr ip4;
  const char* x;

  if ((x = ipv6_scan(fqdn, &ip6)) != 0 && *x == 0) {
    if (dns_result_alloc(out, DNS_T_AAAA, 1, 0) < 0)
      return -1;
    memcpy(out->rr.ip6, &ip6, sizeof ip6);
    return 0;
  }
  if ((x = ipv4_scan(fqdn, &ip4)) != 0 && *x == 0) {
    if (dns_result_alloc(out, DNS_T_AAAA, 1, 0) < 0)
      return -1;
    memcpy(out->rr.ip6[0].addr, IPV6ADDR_V4PREFIX.addr, 12);
    memcpy(out->rr.ip6[0].addr+12, ip4.addr, 4);
    return 0;
  }

  if (!dns_domain_fromdot(&q,fqdn,strlen(fqdn))) return -1;
  if (dns_resolve(tx,q,DNS_T_AAAA) == -1) { free(q); return -1; }
  free(q);
  if (dns_ip6_packet(out,tx->packet,tx->packetlen) == -1)  return -1;
  dns_transmit_free(tx);
  return 0;
}

/** \fn dns_ip6(struct dns_result*, const char*)
    Request the IPv6 address (AAAA) records for a domain name.
*/
DNS_R_FN_WRAP(ip6, const char*)

#ifdef SELFTEST_MAIN
struct dns_result out = {0};
void doit(const char* fqdn)
{
  int i;

  debugfn(dns_ip6(&out, fqdn));
  obuf_putf(&outbuf, "s{ }d{:}", fqdn, out.count);
  for (i = 0; i < out.count; i++) {
    obuf_putc(&outbuf, ' ');
    obuf_puts(&outbuf, ipv6_format(&out.rr.ip6[i]));
  }
  NL();
}
MAIN
{
  doit("1.2.3.4");
  doit("google.ca");
}
#endif
#ifdef SELFTEST_EXP_FNMATCH
result=0
1.2.3.4 1: ::ffff:1.2.3.4
result=0
google.ca 1: 2607:f8b0:400a:*
#endif
