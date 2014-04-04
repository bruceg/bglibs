#include <string.h>

#include "dns.h"

static int doit(str *work,const char *rule)
{
  char ch;
  unsigned int colon;
  unsigned int prefixlen;
  const char *p;

  ch = *rule++;
  if ((ch != '?') && (ch != '=') && (ch != '*') && (ch != '-')) return 1;
  p = strchr(rule,':');
  if (!p) return 1;
  colon = p - rule;

  if (work->len < colon) return 1;
  prefixlen = work->len - colon;
  if ((ch == '=') && prefixlen) return 1;
  if (strncasecmp(rule,work->s + prefixlen,colon)) return 1;
  if (ch == '?') {
    if (memchr(work->s,'.',prefixlen)) return 1;
    if (memchr(work->s,'[',prefixlen)) return 1;
    if (memchr(work->s,']',prefixlen)) return 1;
  }

  work->len = prefixlen;
  if (ch == '-') work->len = 0;
  return str_cats(work,rule + colon + 1);
}

int dns_ip4_qualify_rules(str *out,str *fqdn,const str *in,const str *rules)
{
  unsigned int i;
  unsigned int j;
  unsigned int plus;
  unsigned int fqdnlen;
  const char *p;

  if (!str_copy(fqdn,in)) return -1;

  for (j = i = 0;j < rules->len;++j)
    if (!rules->s[j]) {
      if (!doit(fqdn,rules->s + i)) return -1;
      i = j + 1;
    }

  fqdnlen = fqdn->len;
  p = memchr(fqdn->s,'+',fqdnlen);
  if (!p)
    return dns_ip4(out,fqdn);
  plus = p - fqdn->s;

  i = plus + 1;
  for (;;) {
    p = memchr(fqdn->s + i,'+',fqdnlen - i);
    j = p ? p - (fqdn->s + i) : fqdnlen - i;
    memcpy(fqdn->s + plus,fqdn->s + i,j);
    fqdn->len = plus + j;
    if (dns_ip4(out,fqdn) == -1) return -1;
    if (out->len) return 0;
    i += j;
    if (i >= fqdnlen) return 0;
    ++i;
  }
}

int dns_ip4_qualify(str *out,str *fqdn,const str *in)
{
  static str rules;
  if (dns_resolvconfrewrite(&rules) == -1) return -1;
  return dns_ip4_qualify_rules(out,fqdn,in,&rules);
}
