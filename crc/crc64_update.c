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
  uint64 crc = crc64_block("123456789", 9);
  obuf_putullnumw(&outbuf, crc, 16, '0', 16, obuf_hex_ucase_digits); NL();
}
#endif
#ifdef SELFTEST_EXP
62EC59E3F1A4F00A
#endif
