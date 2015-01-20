#include "dns.h"
#include "resolve.h"

/** Look up the IPv4 address(es) corresponding to a domain name. */
int resolve_ipv4name_n(const char* name, ipv4addr* addr, int maxaddrs)
{
  static struct dns_result out = {0};
  int i;

  if (!resolve_calldns(dns_ip4, &out, name))
    return 0;
  for (i = 0; i < maxaddrs && i < out.count; ++i)
    addr[i] = out.rr.ip4[i];
  return 1;
}

#ifdef SELFTEST_MAIN
MAIN
{
  ipv4addr ips[4];
  int i;
  memset(ips, 0, sizeof ips);
  debugfn(resolve_ipv4name_n("untroubled.org", ips, 4));
  for (i = 0; i < 4; i++)
    obuf_putf(&outbuf, "d{ }s{\n}", i, ipv4_format(&ips[i]));
}
#endif
#ifdef SELFTEST_EXP
result=1
0 69.5.1.51
1 0.0.0.0
2 0.0.0.0
3 0.0.0.0
#endif
