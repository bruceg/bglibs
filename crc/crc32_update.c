#include "crc32.h"

uint32 crc32_update(uint32 crc, const char* data, long len)
{
  const unsigned char* ptr = data;
  while (len-- > 0)
    crc = crc32_table[(crc ^ *ptr++) & 0xff] ^ (crc >> 8);
  return crc;
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
MAIN
{
  uint32 crc = crc32_block("123456789", 9);
  obuf_putXw(&outbuf, crc, 8, '0'); NL();
}
#endif
#ifdef SELFTEST_EXP
CBF43926
#endif
