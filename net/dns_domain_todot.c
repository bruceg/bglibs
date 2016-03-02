#include "dns.h"

/** Translate the domain name in \p d to a dotted text string in \p out. */
int dns_domain_todot_cat(str *out,const char *d)
{
  unsigned int len = fmt_dns_domain(0, d);
  if (!str_realloc(out, out->len + len))
    return 0;
  fmt_dns_domain(out->s + out->len, d);
  out->len += len;
  out->s[out->len] = 0;
  return 1;
}


#ifdef SELFTEST_MAIN
#include "iobuf.h"

void testit(const char* data)
{
  str result = {0};
  debugstrfn(dns_domain_todot_cat(&result, data), &result);
}

MAIN
{
  testit("\007example\003com\000");
  testit("\003joe\007example\003org\000");
}
#endif
#ifdef SELFTEST_EXP
result=1 len=11 size=16 s=example.com
result=1 len=15 size=32 s=joe.example.org
#endif
