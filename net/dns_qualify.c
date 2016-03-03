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
    if (!str_truncate(fqdn, plus + j)) return -1;
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


#ifdef SELFTEST_MAIN

static int skip = 0;
static int dummy(struct dns_transmit* tx, struct dns_result* out, const char* fqdn)
{
  obuf_putf(&outbuf, "{using }s\\\n", fqdn);
  out->count = !skip;
  if (skip > 0)
    --skip;
  return 0;
  (void)tx;
  (void)out;
}

MAIN
{
  struct dns_result out = {0};
  str fqdn = {0};
  str rcfile = {0};

  makefile(&rcfile, "-.local:me.local\n=me:127.0.0.1\n*.a:.af.mil\n?:+.domain1.com+.domain2.net\n*.:\n");
  debugfn(setenv("DNSREWRITEFILE", rcfile.s, 1));
  debugstrfn(dns_qualify(&out,&fqdn,"example.com",dummy), &fqdn);
  debugstrfn(dns_qualify(&out,&fqdn,"sth.local",dummy), &fqdn);
  debugstrfn(dns_qualify(&out,&fqdn,"me",dummy), &fqdn);
  debugstrfn(dns_qualify(&out,&fqdn,"sth.a",dummy), &fqdn);
  debugstrfn(dns_qualify(&out,&fqdn,"sth",dummy), &fqdn);
  skip = 1;
  debugstrfn(dns_qualify(&out,&fqdn,"sth",dummy), &fqdn);
  debugstrfn(dns_qualify(&out,&fqdn,"sth.",dummy), &fqdn);
  debugstrfn(dns_qualify(&out,&fqdn,"a.b+.c.d+.e.f+.g.h",dummy), &fqdn);
  skip = 1;
  debugstrfn(dns_qualify(&out,&fqdn,"a.b+.c.d+.e.f+.g.h",dummy), &fqdn);
  skip = 2;
  debugstrfn(dns_qualify(&out,&fqdn,"a.b+.c.d+.e.f+.g.h",dummy), &fqdn);
  unlink(rcfile.s);
}
#endif
#ifdef SELFTEST_EXP
result=0
using example.com
result=0 len=11 size=16 s=example.com
using me.local
result=0 len=8 size=16 s=me.local
using 127.0.0.1
result=0 len=9 size=16 s=127.0.0.1
using sth.af.mil
result=0 len=10 size=16 s=sth.af.mil
using sth.domain1.com
result=0 len=15 size=48 s=sth.domain1.com
using sth.domain1.com
using sth.domain2.net
result=0 len=15 size=48 s=sth.domain2.net
using sth
result=0 len=3 size=48 s=sth
using a.b.c.d
result=0 len=7 size=48 s=a.b.c.d
using a.b.c.d
using a.b.e.f
result=0 len=7 size=48 s=a.b.e.f
using a.b.c.d
using a.b.e.f
using a.b.g.h
result=0 len=7 size=48 s=a.b.g.h
#endif
