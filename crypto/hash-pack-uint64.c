/* $Id$ */

#include "hash.h"
#include "uint64.h"

void hash_pack_uint64_lsb(uint64 bytes, unsigned char* output)
{
  uint64_pack_lsb(bytes << 3, output);
}

void hash_pack_uint64_msb(uint64 bytes, unsigned char* output)
{
  uint64_pack_msb(bytes << 3, output);
}
