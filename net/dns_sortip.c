#include <string.h>

#include "dns.h"

/* XXX: sort servers by configurable notion of closeness? */
/* XXX: pay attention to competence of each server? */

void dns_sortip(char *s,unsigned int n)
{
  unsigned int i;
  char tmp[4];

  n >>= 2;
  while (n > 1) {
    i = dns_random(n);
    --n;
    memcpy(tmp,s + (i << 2),4);
    memcpy(s + (i << 2),s + (n << 2),4);
    memcpy(s + (n << 2),tmp,4);
  }
}
