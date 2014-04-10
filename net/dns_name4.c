#include <string.h>

#include "dns.h"

/** Request the name (PTR) record for an IPv4 address. */
int dns_name4_r(struct dns_transmit *tx,str *out,const ipv4addr *ip)
{
  char name[DNS_NAME4_DOMAIN];

  dns_name4_domain(name,ip);
  if (dns_resolve(tx,name,DNS_T_PTR) == -1) return -1;
  if (dns_name_packet(out,tx->packet,tx->packetlen) == -1) return -1;
  dns_transmit_free(tx);
  return 0;
}

/** \fn dns_name4(str*, const ipv4addr*)
    Request the name (PTR) record for an IPv4 address.
*/
DNS_R_FN_WRAP2(dns_name4, str*, const ipv4addr*)

#ifdef SELFTEST_MAIN
#include "str/iter.h"
MAIN
{
  str out = {0};
  ipv4addr ip = {{ 69,5,1,51 }};
  striter i;

  dns_name4(&out, &ip);
  striter_loop(&i, &out, 0)
    obuf_putf(&outbuf, "d{: \"}s{\"\n}", i.len, i.startptr);
}
#endif
#ifdef SELFTEST_EXP
14: "untroubled.org"
#endif
