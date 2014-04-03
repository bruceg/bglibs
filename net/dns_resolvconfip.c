#include <stdlib.h>
#include <string.h>

#include "iobuf/ibuf.h"
#include "dns.h"

static str data = {0};

static int init(ipv4addr ip[16])
{
  int i;
  int j;
  int iplen = 0;
  const char *x;

  x = getenv("DNSCACHEIP");
  if (x)
    while (iplen <= 15) {
      if (*x == '.')
	++x;
      else {
        x = ipv4_scan(x,ip + iplen);
	if (!x) break;
	iplen++;
      }
    }

  if (!iplen) {
    i = ibuf_openreadclose("/etc/resolv.conf",&data);
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
            if (iplen <= 15)
              if (ip4_scan(data.s + i,ip + iplen)) {
		if (memcmp(ip + iplen,"\0\0\0\0",4) == 0)
		  memcpy(ip + iplen,"\177\0\0\1",4);
                iplen++;
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
static struct timeval deadline;
static ipv4addr ip[16]; /* defined if ok */

int dns_resolvconfip(ipv4addr s[16])
{
  struct timeval now;

  gettimeofday(&now,0);
  if (TV_LESS(&deadline,&now)) ok = 0;
  if (!uses) ok = 0;

  if (!ok) {
    if (init(ip) == -1) return -1;
    deadline = now;
    deadline.tv_sec += 600;
    uses = 10000;
    ok = 1;
  }

  --uses;
  memcpy(s,ip,64);
  return 0;
}
