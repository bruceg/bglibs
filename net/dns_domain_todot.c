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
