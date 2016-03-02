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
    if (memchr(work->s,':',prefixlen)) return 1;
    if (memchr(work->s,'.',prefixlen)) return 1;
    if (memchr(work->s,'[',prefixlen)) return 1;
    if (memchr(work->s,']',prefixlen)) return 1;
  }

  work->len = prefixlen;
  if (ch == '-') work->len = 0;
  return str_cats(work,rule + colon + 1);
}

/** Qualify a domain name through DNS requests.

    This function is used to qualify what may be a partial domain name
    into a fully qualified domain name through a set of rules and a
    resolver function. The resolution rules are read by \c
    dns_resolvconfrewrite. For each possible qualification of the domain
    name, the resolver function is called to test if the qualified
    domain name exists.

    \param out Output storage for the DNS record results.
    \param fqdn Output storage for the fully qualified domain name.
    \param rules The list of rules to use to qualify the domain name.
    \param in Domain name (full or partial) to qualify.
    \param fn Function to call (such as \c dns_ip4) to determine if a
    qualified domain name exists.
*/
int dns_qualify_rules(struct dns_result* out, str *fqdn, const char *in, const str *rules,
		      int (*fn)(struct dns_transmit*, struct dns_result*, const char*))
{
  unsigned int i;
  unsigned int j;
  unsigned int plus;
  unsigned int fqdnlen;
  const char *p;
  struct dns_transmit tx;

  if (!str_copys(fqdn,in)) return -1;
  memset(&tx, 0, sizeof tx);

  for (j = i = 0;j < rules->len;++j)
    if (!rules->s[j]) {
      if (!doit(fqdn,rules->s + i)) return -1;
      i = j + 1;
    }

  fqdnlen = fqdn->len;
  p = memchr(fqdn->s,'+',fqdnlen);
  if (!p)
    return fn(&tx,out,fqdn->s);
  plus = p - fqdn->s;

  i = plus + 1;
  for (;;) {
    p = memchr(fqdn->s + i,'+',fqdnlen - i);
    j = p ? p - (fqdn->s + i) : fqdnlen - i;
    memmove(fqdn->s + plus,fqdn->s + i,j);
    fqdn->len = plus + j;
    if (fn(&tx,out,fqdn->s) == -1) return -1;
    if (out->count) return 0;
    i += j;
    if (i >= fqdnlen) return 0;
    ++i;
  }
}

/** Qualify a domain name through DNS requests.

    Calls \c dns_resolvconfrewrite and \c dns_qualify_rules.

    \param out Output storage for the DNS record results.
    \param fqdn Output storage for the fully qualified domain name.
    \param in Domain name (full or partial) to qualify.
    \param fn Function to call (such as \c dns_ip4) to determine if a
    qualified domain name exists.
*/
int dns_qualify(struct dns_result* out, str* fqdn, const char* in,
		int (*fn)(struct dns_transmit*, struct dns_result*, const char*))
{
  static str rules;
  if (dns_resolvconfrewrite(&rules) == -1) return -1;
  return dns_qualify_rules(out,fqdn,in,&rules,fn);
}
