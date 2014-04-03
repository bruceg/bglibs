#include <string.h>

#include "taia.h"
#include "openreadclose.h"
#include "ip4.h"
#include "env.h"
#include "dns.h"

static str data = {0};

static int init(char ip[64])
{
  int i;
  int j;
  int iplen = 0;
  char *x;

  x = env_get("DNSCACHEIP");
  if (x)
    while (iplen <= 60) {
      if (*x == '.')
	++x;
      else {
        i = ip4_scan(x,ip + iplen);
	if (!i) break;
	x += i;
	iplen += 4;
      }
    }

  if (!iplen) {
    i = openreadclose("/etc/resolv.conf",&data,64);
    if (i == -1) return -1;
    if (i) {
      if (!str_catc(&data,'\n')) return -1;
      i = 0;
      for (j = 0;j < data.len;++j)
        if (data.s[j] == '\n') {
          if (memcmp("nameserver ",data.s + i,11) == 0 || memcmp("nameserver\t",data.s + i,11) == 0) {
            i += 10;
            while ((data.s[i] == ' ') || (data.s[i] == '\t'))
              ++i;
            if (iplen <= 60)
              if (ip4_scan(data.s + i,ip + iplen)) {
		if (byte_equal(ip + iplen,4,"\0\0\0\0"))
		  byte_copy(ip + iplen,4,"\177\0\0\1");
                iplen += 4;
	      }
          }
          i = j + 1;
        }
    }
  }

  if (!iplen) {
    memcpy(ip,"\177\0\0\1",4);
    iplen = 4;
  }
  memset(ip + iplen,0,64 - iplen);
  return 0;
}

static int ok = 0;
static unsigned int uses;
static struct taia deadline;
static char ip[64]; /* defined if ok */

int dns_resolvconfip(char s[64])
{
  struct taia now;

  taia_now(&now);
  if (taia_less(&deadline,&now)) ok = 0;
  if (!uses) ok = 0;

  if (!ok) {
    if (init(ip) == -1) return -1;
    taia_uint(&deadline,600);
    taia_add(&deadline,&now,&deadline);
    uses = 10000;
    ok = 1;
  }

  --uses;
  memcpy(s,ip,64);
  return 0;
}
