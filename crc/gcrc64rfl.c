/* $Id$ */
#include "gcrc.h"

uint64 gcrc64rfl(uint64 crc, const char* data, long len,
		 const uint64 table[256])
{
  const unsigned char* ptr = (const unsigned char*)data;
  while (len-- > 0)
    crc = table[(crc ^ *ptr++) & 0xff] ^ (crc >> 8);
  return crc;
}
