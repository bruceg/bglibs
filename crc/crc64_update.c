#include "crc64.h"

uint64 crc64_update(uint64 crc, const char* data, long len)
{
  const unsigned char* ptr = data;
  while (len-- > 0)
    crc = crc64_table[(unsigned char)(crc>>56) ^ *ptr++] ^ (crc << 8);
  return crc;
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
MAIN
{
  obuf_putXwll(&outbuf, crc64_block("123456789", 9), 16, '0'); NL();
}
#endif
#ifdef SELFTEST_EXP
62EC59E3F1A4F00A
#endif
