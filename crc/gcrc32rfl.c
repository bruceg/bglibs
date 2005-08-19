/* $Id$ */
#include "gcrc.h"

uint32 gcrc32rfl(uint32 crc, const char* data, long len,
		 const uint32 table[256])
{
  const unsigned char* ptr = data;
  while (len-- > 0)
    crc = table[(crc ^ *ptr++) & 0xff] ^ (crc >> 8);
  return crc;
}

#ifdef SELFTEST_MAIN
#include "crc32.h"
#include "crc32c.h"
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

  /* Extracted from http://lwn.net/Articles/66994/ */
  obuf_putXw(&outbuf, crc32c_block("\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f\x20\x21\x22\x23\x24\x25\x26\x27\x28", 40), 8, '0'); NL();
  obuf_putXw(&outbuf, crc32c_block("\x29\x2a\x2b\x2c\x2d\x2e\x2f\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3a\x3b\x3c\x3d\x3e\x3f\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4a\x4b\x4c\x4d\x4e\x4f\x50", 40), 8, '0'); NL();
  obuf_putXw(&outbuf, crc32c_block("\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5a\x5b\x5c\x5d\x5e\x5f\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6a\x6b\x6c\x6d\x6e\x6f\x70\x71\x72\x73\x74\x75\x76\x77\x78", 40), 8, '0'); NL();
  obuf_putXw(&outbuf, crc32c_block("\x79\x7a\x7b\x7c\x7d\x7e\x7f\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8a\x8b\x8c\x8d\x8e\x8f\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9a\x9b\x9c\x9d\x9e\x9f\xa0", 40), 8, '0'); NL();
  obuf_putXw(&outbuf, crc32c_block("\xa1\xa2\xa3\xa4\xa5\xa6\xa7\xa8\xa9\xaa\xab\xac\xad\xae\xaf\xb0\xb1\xb2\xb3\xb4\xb5\xb6\xb7\xb8\xb9\xba\xbb\xbc\xbd\xbe\xbf\xc0\xc1\xc2\xc3\xc4\xc5\xc6\xc7\xc8", 40), 8, '0'); NL();
  obuf_putXw(&outbuf, crc32c_block("\xc9\xca\xcb\xcc\xcd\xce\xcf\xd0\xd1\xd2\xd3\xd4\xd5\xd6\xd7\xd8\xd9\xda\xdb\xdc\xdd\xde\xdf\xe0\xe1\xe2\xe3\xe4\xe5\xe6\xe7\xe8\xe9\xea\xeb\xec\xed\xee\xef\xf0", 40), 8, '0'); NL();
  obuf_putXw(&outbuf, crc32c_block("\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2a\x2b\x2c\x2d\x2e\x2f\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3a\x3b\x3c\x3d\x3e\x3f\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4a\x4b\x4c\x4d\x4e\x4f\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5a\x5b\x5c\x5d\x5e\x5f\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6a\x6b\x6c\x6d\x6e\x6f\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7a\x7b\x7c\x7d\x7e\x7f\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8a\x8b\x8c\x8d\x8e\x8f\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9a\x9b\x9c\x9d\x9e\x9f\xa0\xa1\xa2\xa3\xa4\xa5\xa6\xa7\xa8\xa9\xaa\xab\xac\xad\xae\xaf\xb0\xb1\xb2\xb3\xb4\xb5\xb6\xb7\xb8\xb9\xba\xbb\xbc\xbd\xbe\xbf\xc0\xc1\xc2\xc3\xc4\xc5\xc6\xc7\xc8\xc9\xca\xcb\xcc\xcd\xce\xcf\xd0\xd1\xd2\xd3\xd4\xd5\xd6\xd7\xd8\xd9\xda\xdb\xdc\xdd\xde\xdf\xe0\xe1\xe2\xe3\xe4\xe5\xe6\xe7\xe8\xe9\xea\xeb\xec\xed\xee\xef\xf0", 240), 8, '0'); NL();
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
0E2C157F
E980EBF6
DE74BDED
D579C862
BA979AD0
2B29D913
24C5D375
#endif
