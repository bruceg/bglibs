#include <stdlib.h>
#include <string.h>

#include "ghash.h"

adt_hash_t ghash_hashs(const unsigned char* data)
{
  adt_hash_t h = 0x1505;
  while (*data != 0)
    h = (h + (h << 5)) ^ *data++;
  return h;
}

adt_hash_t ghash_hashsp(const char* const* p)
{
  return ghash_hashs(*p);
}
