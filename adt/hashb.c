#include <stdlib.h>
#include <string.h>

#include "adt_common.h"

adt_hash_t adt_hashb(const unsigned char* data, unsigned long len)
{
  adt_hash_t h = 0x1505;
  while (len--)
    h = (h + (h << 5)) ^ *data++;
  return h;
}
