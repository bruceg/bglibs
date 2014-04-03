#include <string.h>

#include "dns.h"

/* XXX: sort servers by configurable notion of closeness? */
/* XXX: pay attention to competence of each server? */

void dns_sortip(ipv4addr *s,unsigned int n)
{
  unsigned int i;
  ipv4addr tmp;

  n >>= 2;
  while (n > 1) {
    i = dns_random(n);
    --n;
    memcpy(&tmp,&s[i],sizeof tmp);
    memcpy(&s[i],&s[n],sizeof tmp);
    memcpy(&s[n],&tmp,sizeof tmp);
  }
}
