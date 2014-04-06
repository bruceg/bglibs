#include <string.h>

#include "fmt/number.h"
#include "dns.h"

void dns_name6_domain(char name[DNS_NAME6_DOMAIN],const ipv6addr *addr)
{
  unsigned int i;
  unsigned int j;
  unsigned char a;

  for (j = i = 0; i < 16; ++i, j += 4) {
    a = addr->addr[15-i];
    name[j] = 1;
    name[j+1] = fmt_lcase_digits[a & 0xf];
    name[j+2] = 1;
    name[j+3] = fmt_lcase_digits[a >> 4];
  }
  memcpy(name + 32*2, "\3ip6\4arpa\0", 10);
}
