#include "crc16_xmodem.h"

uint16 crc16_xmodem_update(uint16 crc, const char* data, long len)
{
  const unsigned char* ptr = data;
  while (len-- > 0)
    crc = crc16_xmodem_table[(crc ^ *ptr++) & 0xff] ^ (crc >> 8);
  return crc;
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
MAIN
{
  obuf_putXw(&outbuf, crc16_xmodem_block("123456789", 9), 4, '0'); NL();
}
#endif
#ifdef SELFTEST_EXP
0C73
#endif
