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
#include "str/iter.h"
struct dns_result out = {0};
MAIN
{
  ipv4addr ip = {{ 69,5,1,51 }};
  int i;

  debugfn(dns_name4(&out, &ip));
  for (i = 0; i < out.count; ++i)
    obuf_putf(&outbuf, "d{: \"}s{\"\n}", i, out.rr.name[i]);
}
#endif
#ifdef SELFTEST_EXP
result=0
0: "untroubled.org"
#endif
