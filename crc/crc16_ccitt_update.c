#include "crc16_ccitt.h"

uint16 crc16_ccitt_update(uint16 crc, const char* data, long len)
{
  const unsigned char* ptr = data;
  while (len-- > 0)
    crc = crc16_ccitt_table[(unsigned char)(crc>>8) ^ *ptr++] ^ (crc << 8);
  return crc;
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
MAIN
{
  uint16 crc = crc16_ccitt_block("123456789", 9);
  obuf_putXw(&outbuf, crc, 4, '0'); NL();
}
#endif
#ifdef SELFTEST_EXP
29B1
#endif
