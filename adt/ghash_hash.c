#include <stdlib.h>
#include <string.h>

#include "ghash.h"

unsigned long ghash_hashb(const unsigned char* data, unsigned long len)
{
  unsigned long h = 0x1505;
  while (len--)
    h = (h + (h << 5)) ^ *data++;
  return h;
}

unsigned long ghash_hashs(const unsigned char* s)
{
  return ghash_hashb(s, strlen(s));
}
