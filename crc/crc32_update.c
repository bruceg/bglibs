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
  /* Test vectors gleaned from:
   * ftp://ftp.adelaide.edu.au/pub/rocksoft/crc_v3.txt
   * http://www.di-mgt.com.au/src/basCRC32.txt
   * http://forth.sourceforge.net/algorithm/crc32/crc32-32b.txt
   * http://www.createwindow.com/programming/crc32/crcverify.htm
   * http://www.efg2.com/Lab/Mathematics/CRC.htm
   * http://home.t-online.de/home/uwe.mnich/Wissen/Delphi/Utilities/Utilities.html
   */
  obuf_putXw(&outbuf, crc32_block("123456789", 9), 8, '0'); NL();
  obuf_putXw(&outbuf, crc32_block("", 0), 8, '0'); NL();
  obuf_putXw(&outbuf, crc32_block("a", 1), 8, '0'); NL();
  obuf_putXw(&outbuf, crc32_block("resume", 6), 8, '0'); NL();
  obuf_putXw(&outbuf, crc32_block("abc", 3), 8, '0'); NL();
  obuf_putXw(&outbuf, crc32_block("ABC", 3), 8, '0'); NL();
  obuf_putXw(&outbuf, crc32_block("This is a string", 16), 8, '0'); NL();
  obuf_putXw(&outbuf, crc32_update(0, "1234567890", 1), 8, '0'); NL();
  obuf_putXw(&outbuf, crc32_update(0, "1234567890", 2), 8, '0'); NL();
  obuf_putXw(&outbuf, crc32_update(0, "1234567890", 3), 8, '0'); NL();
  obuf_putXw(&outbuf, crc32_update(0, "1234567890", 4), 8, '0'); NL();
  obuf_putXw(&outbuf, crc32_update(0, "1234567890", 5), 8, '0'); NL();
  obuf_putXw(&outbuf, crc32_update(0, "1234567890", 6), 8, '0'); NL();
  obuf_putXw(&outbuf, crc32_update(0, "1234567890", 7), 8, '0'); NL();
  obuf_putXw(&outbuf, crc32_update(0, "1234567890", 8), 8, '0'); NL();
  obuf_putXw(&outbuf, crc32_update(0, "1234567890", 9), 8, '0'); NL();
  obuf_putXw(&outbuf, crc32_update(0, "1234567890", 10), 8, '0'); NL();
  obuf_putXw(&outbuf, crc32_block("hello world", 11), 8, '0'); NL();
  obuf_putXw(&outbuf, crc32_block("Hello world", 11), 8, '0'); NL();
  obuf_putXw(&outbuf, crc32_block("An Arbitrary String", 19), 8, '0'); NL();
  obuf_putXw(&outbuf, crc32_block("ZYXWVUTSRQPONMLKJIHGFEDBCA", 26), 8, '0'); NL();
}
#endif
#ifdef SELFTEST_EXP
CBF43926
00000000
E8B7BE43
60C1D0A0
352441C2
A3830348
0876633F
51DE003A
0E8A5632
7709BAC0
BAA73FBF
0DD7CD01
B8B072C2
CD6FB6E1
FFC205C6
2DFD2D88
C597C693
0D4A1185
8BD69E52
6FBEAAE7
99CDFDB2
#endif
