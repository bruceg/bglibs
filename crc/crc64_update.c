#include "crc64.h"

uint64 crc64_update(uint64 crc, const char* data, long len)
{
  const unsigned char* ptr = data;
  while (len-- > 0)
    crc = crc64_table[(crc ^ *ptr++) & 0xff] ^ (crc >> 8);
  return crc;
}
