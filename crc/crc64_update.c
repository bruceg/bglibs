#include "crc64.h"

uint64 crc64_update(uint64 crc, const char* data, long len)
{
  const unsigned char* ptr = data;
  while (len-- > 0)
    crc = crc64_table[(unsigned char)(crc>>56) ^ *ptr++] ^ (crc << 8);
  return crc;
}
