#include <string.h>

#include "fmt/number.h"
#include "dns.h"

void dns_name4_domain(char name[DNS_NAME4_DOMAIN],const ipv4addr *ip)
{
  unsigned int namelen;
  unsigned int i;

  namelen = 0;
  i = fmt_udec(name + namelen + 1,(unsigned long) (unsigned char) ip->addr[3]);
  name[namelen++] = i;
  namelen += i;
  i = fmt_udec(name + namelen + 1,(unsigned long) (unsigned char) ip->addr[2]);
  name[namelen++] = i;
  namelen += i;
  i = fmt_udec(name + namelen + 1,(unsigned long) (unsigned char) ip->addr[1]);
  name[namelen++] = i;
  namelen += i;
  i = fmt_udec(name + namelen + 1,(unsigned long) (unsigned char) ip->addr[0]);
  name[namelen++] = i;
  namelen += i;
  memcpy(name + namelen,"\7in-addr\4arpa\0",14);
}
