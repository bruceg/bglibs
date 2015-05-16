#include <stdlib.h>
#include <string.h>

#include "ibuf.h"
#include "misc.h"
#include "striter.h"
#include "dns.h"

static str data = {0};

extern ipv4port dns_use_port;

/** Read the \c /etc/resolv.conf file.
    If \c $DNSRESOLVCONF is set, it names a file to read instead of \c /etc/resolv.conf . */
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

  x = getenv("DNSCACHEPORT");
  if (x && *x) {
    ipv4port p = strtou(x, &x);
    if (*x)
      dns_use_port = p;
  }

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

/** Parse \c /etc/resolv.conf for a list of nameserver IPs. */
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

#ifdef SELFTEST_MAIN
MAIN
{
  ipv4addr ips[DNS_MAX_IPS];
  int i;
  str rcfile = {0};
  makefile(&rcfile, "nameserver 1.2.3.4\nnameserver\t 2.3.4.5\nnameserver \t3.4.5.6");

  debugfn(setenv("DNSRESOLVCONF", rcfile.s, 1));
  debugfn(dns_resolvconfip(ips));
  for (i = 0; i < DNS_MAX_IPS; ++i) {
    obuf_puts(&outbuf, ipv4_format(&ips[i]));
    NL();
  }

  uses = 0;
  debugfn(setenv("DNSCACHEIP", "192.168.1.2,192.168.1.3", 1));
  debugfn(dns_resolvconfip(ips));
  for (i = 0; i < DNS_MAX_IPS; ++i) {
    obuf_puts(&outbuf, ipv4_format(&ips[i]));
    NL();
  }
  unlink(rcfile.s);
}
#endif
#ifdef SELFTEST_EXP
result=0
result=0
1.2.3.4
2.3.4.5
3.4.5.6
0.0.0.0
0.0.0.0
0.0.0.0
0.0.0.0
0.0.0.0
0.0.0.0
0.0.0.0
0.0.0.0
0.0.0.0
0.0.0.0
0.0.0.0
0.0.0.0
0.0.0.0
result=0
result=0
192.168.1.2
192.168.1.3
0.0.0.0
0.0.0.0
0.0.0.0
0.0.0.0
0.0.0.0
0.0.0.0
0.0.0.0
0.0.0.0
0.0.0.0
0.0.0.0
0.0.0.0
0.0.0.0
0.0.0.0
0.0.0.0
#endif
