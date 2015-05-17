#include <errno.h>
#include <string.h>

#include "dns.h"

static int getit(struct dns_result* out, unsigned int i, unsigned int offset,
		 const char* buf, unsigned int len, unsigned int pos, uint16 datalen)
{
  unsigned char header[16];
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
#include "dns-responder.c"
void dump_ip6(int count, const union dns_result_rrs* rr)
{
  int i;

  for (i = 0; i < count; i++) {
    obuf_putc(&outbuf, ' ');
    obuf_puts(&outbuf, ipv6_format(&rr->ip6[i]));
  }
  NL();
}
struct dns_response response1 = {
  1, 1, 0, {
    { "\300\014", 2, 28, 1, 123, "\x26\x07\xf8\xb0\x40\x0b\x08\x0b\x00\x00\x00\x00\x00\x00\x10\x17", 16 },
    { "\300\014", 2, 2, 1, 123456, "\3ns1\6google\3com\0", 16 },
  }
};
MAIN
{
  do_dns_test("1.2.3.4", dns_ip6, dump_ip6);
  do_dns_respond_test("google.ca", &response1, dns_ip6, dump_ip6);
}
#endif
#ifdef SELFTEST_EXP_FNMATCH
result=0
1.2.3.4: count=1
 ::ffff:1.2.3.4
27: ID=XX QR=0 opcode=0 AA=0 TC=0 RD=1 RA=0 Z=0 RCODE=0 QDCOUNT=1 ANCOUNT=0 NSCOUNT=0 ARCOUNT=0
Question: google.ca. QTYPE=28 QCLASS=1
result=0
google.ca: count=1
 2607:f8b0:400b:80b::1017
#endif
