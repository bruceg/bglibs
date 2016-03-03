#include "dns.h"

/** Request the name (PTR) record for an IPv6 address. */
int dns_name6_r(struct dns_transmit *tx, struct dns_result* out, const ipv6addr* ip)
{
  char name[DNS_NAME6_DOMAIN];

  if (ipv6_isv4mapped(ip))
    return dns_name4_r(tx, out, (ipv4addr*)&ip->addr[12]);
  dns_name6_domain(name,ip);
  if (dns_resolve(tx,name,DNS_T_PTR) == -1) return -1;
  if (dns_name_packet(out,tx->packet,tx->packetlen) == -1) return -1;
  dns_transmit_free(tx);
  return 0;
}

/** \fn dns_name6(struct dns_result*, const ipv6addr*)
    Request the name (PTR) record for an IPv6 address.
*/
DNS_R_FN_WRAP(name6, const ipv6addr*)

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
void doit(const ipv6addr* addr)
{
  struct dns_result out = {0};
  debugfn(dns_name6(&out, addr));
  obuf_putf(&outbuf, "s{: count=}d{\n}", ipv6_format(addr), out.count);
  dump_rrs(out.count, &out.rr);
  obuf_flush(&outbuf);
}
MAIN
{
  ipv6addr addr1 = {{ 0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf }};
  ipv6addr addr2 = {{ 0,0,0,0,0,0,0,0,0,0,0xff,0xff,0xab,10,42,99 }};
  start_dns_responder(responses, 2);
  doit(&addr1);
  doit(&addr2);
  wait_dns_responder();
}
#endif
#ifdef SELFTEST_EXP
90: ID=XX QR=0 opcode=0 AA=0 TC=0 RD=1 RA=0 Z=0 RCODE=0 QDCOUNT=1 ANCOUNT=0 NSCOUNT=0 ARCOUNT=0
Question: f.0.e.0.d.0.c.0.b.0.a.0.9.0.8.0.7.0.6.0.5.0.4.0.3.0.2.0.1.0.0.0.ip6.arpa. QTYPE=12 QCLASS=1
result=0
1:203:405:607:809:a0b:c0d:e0f: count=1
 one.example.com
43: ID=XX QR=0 opcode=0 AA=0 TC=0 RD=1 RA=0 Z=0 RCODE=0 QDCOUNT=1 ANCOUNT=0 NSCOUNT=0 ARCOUNT=0
Question: 99.42.10.171.in-addr.arpa. QTYPE=12 QCLASS=1
result=0
::ffff:171.10.42.99: count=1
 two.example.com
#endif
