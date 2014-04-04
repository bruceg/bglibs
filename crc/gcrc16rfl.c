#include "gcrc.h"

uint16 gcrc16rfl(uint16 crc, const char* data, long len,
		 const uint16 table[256])
{
  const unsigned char* ptr = (const unsigned char*)data;
  while (len-- > 0)
    crc = table[(crc ^ *ptr++) & 0xff] ^ (crc >> 8);
  return crc;
}

#ifdef SELFTEST_MAIN
#include "crc16_arc.h"
#include "crc16_xmodem.h"
MAIN
{
  /* Test vectors gleaned from:
   * ftp://ftp.adelaide.edu.au/pub/rocksoft/crc_v3.txt
   * http://www.netrino.com/Connecting/2000-01/
   * http://www.efg2.com/Lab/Mathematics/FelipeRochaMachado.TXT
   * http://www.efg2.com/Lab/Mathematics/CRC.htm
   */
  obuf_putXw(&outbuf, crc16_arc_block("123456789", 9), 4, '0'); NL();
  obuf_putXw(&outbuf, crc16_arc_block("abc", 3), 4, '0'); NL();
  obuf_putXw(&outbuf, crc16_arc_block("ABC", 3), 4, '0'); NL();
  obuf_putXw(&outbuf, crc16_arc_block("This is a string", 16), 4, '0'); NL();

  obuf_putXw(&outbuf, crc16_xmodem_block("123456789", 9), 4, '0'); NL();
}
#endif
#ifdef SELFTEST_EXP
BB3D
9738
4521
4C44
0C73
#endif
