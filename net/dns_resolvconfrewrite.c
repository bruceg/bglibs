#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ibuf.h"
#include "striter.h"
#include "dns.h"

static str data = {0};

static int init(str *rules)
{
  char host[256];
  const char *x;
  int i;
  int j;
  int k;
  striter s;

  if (!str_copys(rules,"")) return -1;

  x = getenv("DNSREWRITEFILE");
  if (!x) x = "/etc/dnsrewrite";

  i = ibuf_openreadclose(x,&data);
  if (i == -1) return -1;

  if (i) {
    if (!str_catc(&data,'\n')) return -1;
    striter_loop(&s, &data, '\n') {
      if (!str_catb(rules, s.startptr, s.len)) return -1;
      while (rules->len) {
	if (rules->s[rules->len - 1] != ' '
	    && rules->s[rules->len - 1] != '\t'
	    && rules->s[rules->len - 1] != '\r')
	  break;
	--rules->len;
      }
      if (!str_catc(rules,0)) return -1;
    }
    return 0;
  }

  x = getenv("LOCALDOMAIN");
  if (x) {
    if (!str_copys(&data,x)) return -1;
    if (!str_catc(&data,' ')) return -1;
    if (!str_copys(rules,"?:")) return -1;
    striter_loop(&s, &data, ' ') {
      if (!str_cats(rules, "+.")) return -1;
      if (!str_catb(rules, s.startptr, s.len)) return -1;
    }
    if (!str_catc(rules,0)) return -1;
    if (!str_cats(rules,"*.:")) return -1;
    if (!str_catc(rules,0)) return -1;
    return 0;
  }

  i = dns_read_resolvconf(&data);
  if (i == -1) return -1;

  if (i) {
    if (!str_catc(&data,'\n')) return -1;
    striter_loop(&s, &data, '\n') {
      if (memcmp("search ", s.startptr, 7) == 0
	  || memcmp("search\t", s.startptr, 7) == 0
	  || memcmp("domain ", s.startptr, 7) == 0
	  || memcmp("domain\t", s.startptr, 7) == 0) {
	if (!str_copys(rules,"?:")) return -1;
	i = 7;
	j = s.len;
	while (i < j) {
	  x = memchr(s.startptr + i,' ',j - i);
	  k = x ? x - (s.startptr + i) : j - i;
	  x = memchr(s.startptr + i,'\t',k);
	  k = x ? x - (s.startptr + i) : k;
	  if (!k) { ++i; continue; }
	  if (!str_cats(rules,"+.")) return -1;
	  if (!str_catb(rules, s.startptr + i, k)) return -1;
	  i += k;
	}
	if (!str_catc(rules,0)) return -1;
	if (!str_cats(rules,"*.:")) return -1;
	if (!str_catc(rules,0)) return -1;
	return 0;
      }
    }
  }

  host[0] = 0;
  if (gethostname(host,sizeof host) == -1) return -1;
  host[(sizeof host) - 1] = 0;
  x = strchr(host,'.');
  if (x) {
    if (!str_copys(rules,"?:")) return -1;
    if (!str_cats(rules,host + i)) return -1;
    if (!str_catc(rules,0)) return -1;
  }
  if (!str_cats(rules,"*.:")) return -1;
  if (!str_catc(rules,0)) return -1;

  return 0;
}

static int ok = 0;
static unsigned int uses;
static struct timeval deadline;
static str rules = {0}; /* defined if ok */

/** Load the domain name qualification rules.

    The rules are loaded from the file named by \c $DNSREWRITEFILE
    (defaults to "/etc/dnsrewrite").

    For compatibility, if the rewriting rules file is not present, the
    qualification procedure looks for a local domain name in three
    places:

    -# The value of the \c $LOCALDOMAIN environment variable, if it is set.
    -# The first \c domain or \c search line in \c /etc/resolv.conf (if any).
    -# Everything after the first period in the system's hostname.

    It translate these into instructions that add the local domain name
    to any name without dots or brackets.

    See http://cr.yp.to/djbdns/qualify.html for complete details.
*/
int dns_resolvconfrewrite(str *out)
{
  struct timeval now;

  gettimeofday(&now,0);
  if (TV_LESS(&deadline,&now)) ok = 0;
  if (!uses) ok = 0;

  if (!ok) {
    if (init(&rules) == -1) return -1;
    deadline = now;
    deadline.tv_sec += 600;
    uses = 10000;
    ok = 1;
  }

  --uses;
  if (!str_copy(out,&rules)) return -1;
  return 0;
}

#ifdef SELFTEST_MAIN
MAIN
{
  str rcfile = {0};

  makefile(&rcfile, "search \tdomain1.com\ndomain domain2.com");
  debugfn(setenv("DNSRESOLVCONF", rcfile.s, 1));
  debugstrfn(dns_resolvconfrewrite(&rules), &rules);
  unlink(rcfile.s);

  uses = 0; data.len = 0;
  debugfn(setenv("LOCALDOMAIN", "local.domain", 1));
  debugstrfn(dns_resolvconfrewrite(&rules), &rules);

  uses = 0; data.len = 0;
  makefile(&rcfile, "-.local:me.local\n=me:127.0.0.1\n*.a:.af.mil\n?:.heaven.af.mil\n*.:\n");
  debugfn(setenv("DNSREWRITEFILE", rcfile.s, 1));
  debugstrfn(dns_resolvconfrewrite(&rules), &rules);
  unlink(rcfile.s);
}
#endif
#ifdef SELFTEST_EXP
result=0
result=0 len=20 size=32 s=?:+.domain1.com^@*.:^@
result=0
result=0 len=21 size=32 s=?:+.local.domain^@*.:^@
result=0
result=0 len=65 size=80 s=-.local:me.local^@=me:127.0.0.1^@*.a:.af.mil^@?:.heaven.af.mil^@*.:^@^@
#endif
