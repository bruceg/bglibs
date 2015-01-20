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
#include "str/iter.h"
struct dns_result out = {0};
void doit(const char* addr)
{
  ipv6addr ip;
  int i;

  ipv6_scan(addr, &ip);
  debugfn(dns_name6(&out, &ip));
  for (i = 0; i < out.count; ++i)
    obuf_putf(&outbuf, "d{: \"}s{\"\n}", i, out.rr.name[i]);
}
MAIN
{
  doit("::ffff:69.5.1.51");
  doit("2620::861:ed1a:0:0:0:1");
}

#endif
#ifdef SELFTEST_EXP
result=0
0: "untroubled.org"
result=0
0: "text-lb.eqiad.wikimedia.org"
#endif
