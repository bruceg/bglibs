#include <stdlib.h>
#include <string.h>

#include "ghash.h"

unsigned long ghash_hashs(const unsigned char* data)
{
  unsigned long h = 0x1505;
  while (*data != 0)
    h = (h + (h << 5)) ^ *data++;
  return h;
}

unsigned long ghash_hashsp(const char* const* p)
{
  return ghash_hashs(*p);
}
