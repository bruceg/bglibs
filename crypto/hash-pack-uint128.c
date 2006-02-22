/* $Id$ */

#include "hash.h"
#include "uint64.h"

void hash_pack_uint128_msb(uint64 bytes, unsigned char* output)
{
  uint64_pack_msb(bytes >> 61, output);
  uint64_pack_msb(bytes << 3, output + 8);
}
