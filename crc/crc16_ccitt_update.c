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
  obuf_putXw(&outbuf, crc16_ccitt_block("123456789", 9), 4, '0'); NL();
  obuf_putXw(&outbuf, crc16_ccitt_update(0, "1234567890", 1), 4, '0'); NL();
  obuf_putXw(&outbuf, crc16_ccitt_update(0, "1234567890", 2), 4, '0'); NL();
  obuf_putXw(&outbuf, crc16_ccitt_update(0, "1234567890", 3), 4, '0'); NL();
  obuf_putXw(&outbuf, crc16_ccitt_update(0, "1234567890", 4), 4, '0'); NL();
  obuf_putXw(&outbuf, crc16_ccitt_update(0, "1234567890", 5), 4, '0'); NL();
  obuf_putXw(&outbuf, crc16_ccitt_update(0, "1234567890", 6), 4, '0'); NL();
  obuf_putXw(&outbuf, crc16_ccitt_update(0, "1234567890", 7), 4, '0'); NL();
  obuf_putXw(&outbuf, crc16_ccitt_update(0, "1234567890", 8), 4, '0'); NL();
  obuf_putXw(&outbuf, crc16_ccitt_update(0, "1234567890", 9), 4, '0'); NL();
  obuf_putXw(&outbuf, crc16_ccitt_update(0, "1234567890", 10), 4, '0'); NL();
}
#endif
#ifdef SELFTEST_EXP
29B1
2672
20B5
9752
D789
546C
20E4
86D6
9015
31C3
D321
#endif
