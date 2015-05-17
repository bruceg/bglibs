#include <errno.h>
#include <string.h>

#include "dns.h"

static int getit(struct dns_result* out, unsigned int i, unsigned int offset,
		 const char* buf, unsigned int len, unsigned int pos, uint16 datalen)
{
  unsigned char header[4];
  if (datalen != 4) { errno = EPROTO; return -1; }
  if (!dns_packet_copy(buf,len,pos,header,4)) { errno = EPROTO; return -1; }
  memcpy(&out->rr.ip4[i], header, 4);
  return 0;
  (void)offset;
}

/** Extract IPv4 address (A) records from a DNS response packet. */
int dns_ip4_packet(struct dns_result* out, const char* buf, unsigned int len)
{
  if (dns_packet_extract(out, buf, len, DNS_T_A, DNS_C_IN, 0, getit) < 0)
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
    if (dns_result_alloc(out, DNS_T_A, 1, 0) < 0)
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
#include "dns-responder.c"
void dump_ip4(int count, const union dns_result_rrs* rr)
{
  int i;
  for (i = 0; i < count; i++) {
    obuf_putc(&outbuf, ' ');
    obuf_puts(&outbuf, ipv4_format(&rr->ip4[i]));
  }
  NL();
}
struct dns_response response1 = {
  1, 1, 0, {
    { "\300\014", 2, 1, 1, 512, "\105\005\001\227", 4 },
    { "\300\014", 2, 2, 1, 123456, "\003ns1\012untroubled\3org\0", 20 },
  }
};
struct dns_response response2 = {
  3, 1, 0, {
    { "\300\014", 2, 1, 1, 512, "\105\005\001\227", 4 },
    { "\300\014", 2, 1, 1, 512, "\105\005\001\226", 4 },
    { "\300\014", 2, 1, 1, 512, "\105\005\001\225", 4 },
    { "\300\014", 2, 2, 1, 123456, "\003ns1\012untroubled\3org\0", 20 },
  }
};
MAIN
{
  do_dns_test("1.2.3.4", dns_ip4, dump_ip4);
  do_dns_respond_test("example.com", &response1, dns_ip4, dump_ip4);
  do_dns_respond_test("example.org", &response2, dns_ip4, dump_ip4);
}
#endif
#ifdef SELFTEST_EXP
result=0
1.2.3.4: count=1
 1.2.3.4
29: ID=XX QR=0 opcode=0 AA=0 TC=0 RD=1 RA=0 Z=0 RCODE=0 QDCOUNT=1 ANCOUNT=0 NSCOUNT=0 ARCOUNT=0
Question: example.com. QTYPE=1 QCLASS=1
result=0
example.com: count=1
 69.5.1.151
29: ID=XX QR=0 opcode=0 AA=0 TC=0 RD=1 RA=0 Z=0 RCODE=0 QDCOUNT=1 ANCOUNT=0 NSCOUNT=0 ARCOUNT=0
Question: example.org. QTYPE=1 QCLASS=1
result=0
example.org: count=3
 69.5.1.149 69.5.1.150 69.5.1.151
#endif
