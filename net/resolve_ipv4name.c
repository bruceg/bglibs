#include "dns.h"
#include "resolve.h"

/** Look up the IPv4 address(es) corresponding to a domain name. */
int resolve_ipv4name_n(const char* name, ipv4addr* addr, int maxaddrs)
{
  static struct dns_result out = {0};
  int i;

  if (!resolve_qualdns(dns_ip4_r, &out, name))
    return 0;
  for (i = 0; i < maxaddrs && i < out.count; ++i)
    addr[i] = out.rr.ip4[i];
  return 1;
}

#ifdef SELFTEST_MAIN
#include "dns-responder.c"
RESPONSE response1 = {
  2, 1, 0, {
    { "\300\014", 2, DNS_T_A, DNS_C_IN, 86400, "\002\004\006\010", 4 },
    { "\300\014", 2, DNS_T_A, DNS_C_IN, 86400, "\002\004\006\011", 4 },
    { "\300\014", 2, DNS_T_NS, DNS_C_IN, 123456, "\003ns1\007example\003org\0", 17},
  }
};
MAIN
{
  ipv4addr ips[4];
  int i;
  memset(ips, 0, sizeof ips);
  start_dns_responder(&response1, 1);
  setenv("LOCALDOMAIN", "example.org", 1);
  debugfn(resolve_ipv4name_n("test", ips, 4));
  for (i = 0; i < 4; i++)
    obuf_putf(&outbuf, "d{ }s{\n}", i, ipv4_format(&ips[i]));
  wait_dns_responder();
}
NODUMP
#endif
#ifdef SELFTEST_EXP
34: ID=XX QR=0 opcode=0 AA=0 TC=0 RD=1 RA=0 Z=0 RCODE=0 QDCOUNT=1 ANCOUNT=0 NSCOUNT=0 ARCOUNT=0
Question: test.example.org. QTYPE=1 QCLASS=1
result=1
0 2.4.6.9
1 2.4.6.8
2 0.0.0.0
3 0.0.0.0
#endif
