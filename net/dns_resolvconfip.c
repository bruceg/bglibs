#include <stdlib.h>
#include <string.h>

#include "iobuf/ibuf.h"
#include "str/iter.h"
#include "dns.h"

static str data = {0};

int dns_read_resolvconf(str *out)
{
  const char *x;
  x = getenv("DNSRESOLVCONF");
  if (!x)
    x = "/etc/resolv.conf";
  return ibuf_openreadclose(x, out);
}

static int init(ipv4addr ip[DNS_MAX_IPS])
{
  int i;
  striter j;
  int iplen = 0;
  const char *x;

  x = getenv("DNSCACHEIP");
  if (x)
    while (iplen <= 15) {
      if (*x == ',')
	++x;
      else {
        x = ipv4_scan(x,ip + iplen);
	if (!x) break;
	iplen++;
      }
    }

  if (!iplen) {
    i = dns_read_resolvconf(&data);
    if (i == -1) return -1;
    if (i) {
      if (!str_catc(&data,'\n')) return -1;
      striter_loop(&j, &data, '\n') {
	if (memcmp("nameserver ", j.startptr, 11) == 0 || memcmp("nameserver\t", j.startptr, 11) == 0) {
	  for (i = j.start + 10; data.s[i] == ' ' || data.s[i] == '\t'; ++i)
	    ;
	  if (iplen <= 15)
	    if (ipv4_scan(data.s + i,ip + iplen)) {
	      if (memcmp(ip + iplen,"\0\0\0\0",4) == 0)
		memcpy(ip + iplen,"\177\0\0\1",4);
	      iplen++;
	    }
	}
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
static ipv4addr ip[DNS_MAX_IPS]; /* defined if ok */

int dns_resolvconfip(ipv4addr s[DNS_MAX_IPS])
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
