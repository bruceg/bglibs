#include "gcrc.h"

uint16 gcrc16fwd(uint16 crc, const char* data, long len,
		 const uint16 table[256])
{
  const unsigned char* ptr = data;
  while (len-- > 0)
    crc = table[((crc >> 8) ^ *ptr++) & 0xff] ^ (crc << 8);
  return crc;
}

#ifdef SELFTEST_MAIN
#include "crc16_ccitt.h"
#include "selftest.c"
MAIN
{
  /* Test vectors gleaned from:
   * http://www.netrino.com/Connecting/2000-01/
   * http://home.t-online.de/home/uwe.mnich/Wissen/Delphi/Utilities/Utilities.html
   */
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
