#include <stdlib.h>
#include <string.h>

#include "ghash.h"

adt_hash_t ghash_hashb(const unsigned char* data, unsigned long len)
{
  adt_hash_t h = 0x1505;
  while (len--)
    h = (h + (h << 5)) ^ *data++;
  return h;
}
