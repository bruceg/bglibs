#include <stdlib.h>
#include <string.h>

#include "common.h"

adt_hash_t adt_hashs(const unsigned char* data)
{
  adt_hash_t h = 0x1505;
  while (*data != 0)
    h = (h + (h << 5)) ^ *data++;
  return h;
}
