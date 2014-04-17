#include "dns.h"
#include "resolve.h"

/** Look up the domain name corresponding to an IPv4 address. */
const char* resolve_ipv4addr(const ipv4addr* addr)
{
  static struct dns_result out = {0};
  if (!resolve_calldns(dns_name4, &out, addr))
    return 0;
  return out.rr.name[0];
}

#ifdef SELFTEST_MAIN
MAIN
{
  ipv4addr ip = {{ 69,5,1,51 }};
  obuf_putf(&outbuf, "s{ => }s{\n}", ipv4_format(&ip), resolve_ipv4addr(&ip));
}
#endif
#ifdef SELFTEST_EXP
69.5.1.51 => untroubled.org
#endif
