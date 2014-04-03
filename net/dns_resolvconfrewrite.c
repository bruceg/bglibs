#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "iobuf/ibuf.h"
#include "taia.h"
#include "dns.h"

static str data = {0};

static int init(str *rules)
{
  char host[256];
  const char *x;
  int i;
  int j;
  int k;

  if (!str_copys(rules,"")) return -1;

  x = getenv("DNSREWRITEFILE");
  if (!x) x = "/etc/dnsrewrite";

  i = ibuf_openreadclose(x,&data);
  if (i == -1) return -1;

  if (i) {
    if (!str_catc(&data,'\n')) return -1;
    i = 0;
    for (j = 0;j < data.len;++j)
      if (data.s[j] == '\n') {
        if (!str_catb(rules,data.s + i,j - i)) return -1;
        while (rules->len) {
          if (rules->s[rules->len - 1] != ' ')
          if (rules->s[rules->len - 1] != '\t')
          if (rules->s[rules->len - 1] != '\r')
            break;
          --rules->len;
        }
        if (!str_catc(rules,0)) return -1;
        i = j + 1;
      }
    return 0;
  }

  x = getenv("LOCALDOMAIN");
  if (x) {
    if (!str_copys(&data,x)) return -1;
    if (!str_catc(&data,' ')) return -1;
    if (!str_copys(rules,"?:")) return -1;
    i = 0;
    for (j = 0;j < data.len;++j)
      if (data.s[j] == ' ') {
        if (!str_cats(rules,"+.")) return -1;
        if (!str_catb(rules,data.s + i,j - i)) return -1;
        i = j + 1;
      }
    if (!str_catc(rules,0)) return -1;
    if (!str_cats(rules,"*.:")) return -1;
    if (!str_catc(rules,0)) return -1;
    return 0;
  }

  i = ibuf_openreadclose("/etc/resolv.conf",&data);
  if (i == -1) return -1;

  if (i) {
    if (!str_catc(&data,'\n')) return -1;
    i = 0;
    for (j = 0;j < data.len;++j)
      if (data.s[j] == '\n') {
        if (memcmp("search ",data.s + i,7) == 0 || memcmp("search\t",data.s + i,7) == 0 || memcmp("domain ",data.s + i,7) == 0 || memcmp("domain\t",data.s + i,7) == 0) {
          if (!str_copys(rules,"?:")) return -1;
          i += 7;
          while (i < j) {
	    x = memchr(data.s + i,' ',j - i);
	    k = x ? x - (data.s + i) : j - i;
	    x = memchr(data.s + i,'\t',k);
	    k = x ? x - (data.s + i) : k;
            if (!k) { ++i; continue; }
            if (!str_cats(rules,"+.")) return -1;
            if (!str_catb(rules,data.s + i,k)) return -1;
            i += k;
          }
          if (!str_catc(rules,0)) return -1;
          if (!str_cats(rules,"*.:")) return -1;
          if (!str_catc(rules,0)) return -1;
          return 0;
        }
        i = j + 1;
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
static struct taia deadline;
static str rules = {0}; /* defined if ok */

int dns_resolvconfrewrite(str *out)
{
  struct taia now;

  taia_now(&now);
  if (taia_less(&deadline,&now)) ok = 0;
  if (!uses) ok = 0;

  if (!ok) {
    if (init(&rules) == -1) return -1;
    taia_uint(&deadline,600);
    taia_add(&deadline,&now,&deadline);
    uses = 10000;
    ok = 1;
  }

  --uses;
  if (!str_copy(out,&rules)) return -1;
  return 0;
}
