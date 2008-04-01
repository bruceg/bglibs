#include "gcrc.h"

uint32 gcrc32fwd(uint32 crc, const char* data, long len,
		 const uint32 table[256])
{
  const unsigned char* ptr = (const unsigned char*)data;
  while (len-- > 0)
    crc = table[((crc >> 24) ^ *ptr++) & 0xff] ^ (crc << 8);
  return crc;
}
