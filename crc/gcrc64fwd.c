/* $Id$ */
#include "gcrc.h"

uint64 gcrc64fwd(uint64 crc, const char* data, long len,
		 const uint64 table[256])
{
  const unsigned char* ptr = (const unsigned char*)data;
  while (len-- > 0)
    crc = table[((crc >> 56) ^ *ptr++) & 0xff] ^ (crc << 8);
  return crc;
}

#ifdef SELFTEST_MAIN
#include "crc64.h"
#include "selftest.c"
MAIN
{
  obuf_putXwll(&outbuf, crc64_block("123456789", 9), 16, '0'); NL();
}
#endif
#ifdef SELFTEST_EXP
62EC59E3F1A4F00A
#endif
