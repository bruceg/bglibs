#include <string.h>

#include "dns.h"

/** Request the name (PTR) record for an IPv4 address. */
int dns_name4_r(struct dns_transmit* tx, struct dns_result* out, const ipv4addr *ip)
{
  char name[DNS_NAME4_DOMAIN];

  dns_name4_domain(name,ip);
  if (dns_resolve(tx,name,DNS_T_PTR) == -1) return -1;
  if (dns_name_packet(out,tx->packet,tx->packetlen) == -1) return -1;
  dns_transmit_free(tx);
  return 0;
}

/** \fn dns_name4(struct dns_result*, const ipv4addr*)
    Request the name (PTR) record for an IPv4 address.
*/
DNS_R_FN_WRAP(name4, const ipv4addr*)

#ifdef SELFTEST_MAIN
#include "dns-responder.c"
DUMP {
  int i;
  for (i = 0; i < count; i++) {
    obuf_putc(&outbuf, ' ');
    obuf_puts(&outbuf, rr->name[i]);
  }
  NL();
}
RESPONSE responses[] = {
  { 1, 1, 0, {
      { "\300\014", 2, DNS_T_PTR, DNS_C_IN, 86400, "\003one\007example\003com\000", 17 },
      { "\300\014", 2, DNS_T_NS, DNS_C_IN, 123456, "\003ns1\007example\003org\000", 17 },
    }
  },
  { 1, 1, 0, {
      { "\300\014", 2, DNS_T_PTR, DNS_C_IN, 86400, "\003two\007example\003com\000", 17 },
      { "\300\014", 2, DNS_T_NS, DNS_C_IN, 123456, "\003ns1\007example\003com\000", 17 },
    }
  },
};
void doit(const ipv4addr* addr)
{
  struct dns_result out = {0};
  debugfn(dns_name4(&out, addr));
  obuf_putf(&outbuf, "s{: count=}d{\n}", ipv4_format(addr), out.count);
  dump_rrs(out.count, &out.rr);
  obuf_flush(&outbuf);
}
MAIN
{
  ipv4addr addr1 = {{ 1,4,2,8 }};
  ipv4addr addr2 = {{ 9,8,7,6 }};
  start_dns_responder(responses, 2);
  doit(&addr1);
  doit(&addr2);
  wait_dns_responder();
}
#endif
#ifdef SELFTEST_EXP
38: ID=XX QR=0 opcode=0 AA=0 TC=0 RD=1 RA=0 Z=0 RCODE=0 QDCOUNT=1 ANCOUNT=0 NSCOUNT=0 ARCOUNT=0
Question: 8.2.4.1.in-addr.arpa. QTYPE=12 QCLASS=1
result=0
1.4.2.8: count=1
 one.example.com
38: ID=XX QR=0 opcode=0 AA=0 TC=0 RD=1 RA=0 Z=0 RCODE=0 QDCOUNT=1 ANCOUNT=0 NSCOUNT=0 ARCOUNT=0
Question: 6.7.8.9.in-addr.arpa. QTYPE=12 QCLASS=1
result=0
9.8.7.6: count=1
 two.example.com
#endif
