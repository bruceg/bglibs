#include "crc32.h"

uint32 crc32_update(uint32 crc, const char* data, long len)
{
  const unsigned char* ptr = data;
  while (len-- > 0)
    crc = crc32_table[(crc ^ *ptr++) & 0xff] ^ (crc >> 8);
  return crc;
}
