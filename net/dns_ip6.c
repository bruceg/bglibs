#include <string.h>

#include "dns.h"

static int getit(str* out, const char* buf, unsigned int len, unsigned int pos, uint16 datalen)
{
  unsigned char header[4];
  if (datalen == 16) {
    if (!dns_packet_copy(buf,len,pos,header,16)) return -1;
    if (!str_catb(out,(char*)header,16)) return -1;
  }
  return 1;
  (void)datalen;
}

/** Extract IPv6 address (AAAA) records from a DNS response packet. */
int dns_ip6_packet(str *out,const char *buf,unsigned int len)
{
  if (dns_packet_extract(out, buf, len, DNS_T_AAAA, DNS_C_IN, getit) < 0)
    return -1;
  dns_rotateipv6((ipv6addr*)out->s,out->len/16);
  return 0;
}

/** Request the IPv6 address (AAAA) records for a domain name. */
int dns_ip6_r(struct dns_transmit *tx,str *out,const char *fqdn)
{
  char *q = 0;
  ipv6addr ip6;
  ipv4addr ip4;
  const char* x;

  if ((x = ipv6_scan(fqdn, &ip6)) != 0 && *x == 0) {
    if (!str_copyb(out, (char*)ip6.addr, 16)) return -1;
    return 0;
  }
  if ((x = ipv4_scan(fqdn, &ip4)) != 0 && *x == 0) {
    memcpy(ip6.addr, IPV6ADDR_V4PREFIX.addr, 12);
    memcpy(ip6.addr+12, ip4.addr, 4);
    if (!str_copyb(out, (char*)ip6.addr, 16)) return -1;
    return 0;
  }

  if (!dns_domain_fromdot(&q,fqdn,strlen(fqdn))) return -1;
  if (dns_resolve(tx,q,DNS_T_AAAA) == -1) { free(q); return -1; }
  free(q);
  if (dns_ip6_packet(out,tx->packet,tx->packetlen) == -1)  return -1;
  dns_transmit_free(tx);
  return 0;
}

/** \fn dns_ip6(str*, const char*)
    Request the IPv6 address (AAAA) records for a domain name.
*/
DNS_R_FN_WRAP2(dns_ip6, str*, const char*)

#ifdef SELFTEST_MAIN
void doit(const char* fqdn)
{
  str out = {0};
  unsigned int i;

  dns_ip6(&out, fqdn);
  obuf_putf(&outbuf, "s{ }d{:}", fqdn, out.len);
  for (i = 0; i+16 <= out.len; i += 16) {
    obuf_putc(&outbuf, ' ');
    obuf_puts(&outbuf, ipv6_format((ipv6addr*)out.s + i));
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
1.2.3.4 16: ::ffff:1.2.3.4
google.ca 16: 2607:f8b0:400a:*
#endif
