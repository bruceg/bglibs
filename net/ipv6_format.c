#include <string.h>

#include "ipv6.h"

static char hex[16] = "0123456789ABCDEF";

static char* format_part(uint16 i, char* str)
{
  if (i > 0xfff) *str++ = hex[(i >> 12) & 0xf];
  if (i > 0xff)  *str++ = hex[(i >> 8) & 0xf];
  if (i > 0xf)   *str++ = hex[(i >> 4) & 0xf];
  *str++ = hex[i & 0xf];
  return str;
}

static char* format_ipv4(char* buf, const ipv6addr* addr)
{
  int i;
  for (i = 12; i < 16; ++i) {
    const unsigned j = addr->addr[i];
    if (i > 12)
      *buf++ = '.';
    if (j >= 100)
      *buf++ = (j / 100) + '0';
    if (j >= 10)
      *buf++ = ((j / 10) % 10) + '0';
    *buf++ = (j % 10) + '0';
  }
  return buf;
}

/** Produce a formatted C string from an IPv6 address.

\note The return value is statically allocated.  Multiple calls to this
function will return pointers to the same string.
*/
const char* ipv6_format(const ipv6addr* addr)
{
  uint16 bits[8];
  static char buf[40];
  char* str = buf;
  int i;
  int first;

  for (i = 0; i < 8; ++i)
    bits[i] = addr->addr[i*2] << 8 | addr->addr[i*2+1];
  for (first = 0; first < 8; ++first)
    if (bits[first] != 0)
      break;
  /* A couple of special cases, to simplify IPv4 style output */
  if (first >= 8)
    return "::";
  if (first == 7 && bits[7] == 1)
    return "::1";
  if (first >= 6) {
    strcpy(buf, "::");
    str = format_ipv4(buf+2, addr);
  }
  else if (first == 5 && bits[5] == 0xffff) {
    strcpy(buf, "::FFFF:");
    str = format_ipv4(buf+7, addr);
  }
  else if (first > 1) {
    *str++ = ':';
    for (i = first; i < 8; ++i) {
      *str++ = ':';
      str = format_part(bits[i], str);
    }
  }
  else {
    int zs = 8;
    int ze = 8;
    /* Find the largest internal run of zeros */
    for (i = 1; i < 8; ++i)
      if (bits[i] == 0) {
	const int z = i;
	for (++i; i < 8 && bits[i] == 0; ++i) ;
	if (i-z > ze-zs)
	  zs = z, ze = i;
      }
    for (i = 0; i < zs; ++i) {
      if (i > 0)
	*str++ = ':';
      str = format_part(bits[i], str);
    }
    if (zs < 8) {
      *str++ = ':';
      *str++ = ':';
      for (i = ze; i < 8; ++i) {
	if (i > ze)
	  *str++ = ':';
	str = format_part(bits[i], str);
      }
    }
  }
  *str = 0;
  return buf;
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
static void test(const unsigned char a[16])
{
  ipv6addr ip;
  int i;
  for (i = 0; i < 16; ++i)
    obuf_putxw(&outbuf, a[i], 2, '0');
  obuf_putc(&outbuf, '=');
  memcpy(&ip, a, 16);
  obuf_put2s(&outbuf, ipv6_format(&ip), "\n");
}

MAIN
{
  test("\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00");
  test("\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01");
  test("\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x02");
  test("\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xff\xff\x01\x14\x64\xc8");
  test("\x00\x00\x00\x00\x01\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00");
  test("\x10\x80\x00\x00\x00\x00\x00\x00\x00\x08\x08\x00\x20\x0c\x41\x7a");
  test("\xff\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x01");
  test("\xff\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00");
  test("\x01\x12\x23\x34\x45\x56\x67\x78\x89\x9a\xab\xbc\xcd\xde\xef\xf0");
}
#endif
#ifdef SELFTEST_EXP
00000000000000000000000000000000=::
00000000000000000000000000000001=::1
00000000000000000000000000000002=::0.0.0.2
00000000000000000000ffff011464c8=::FFFF:1.20.100.200
00000000010100000000000000000000=::101:0:0:0:0:0
108000000000000000080800200c417a=1080::8:800:200C:417A
ff010000000000000000000000000101=FF01::101
ff010000000000000000000000000000=FF01::
0112233445566778899aabbccddeeff0=112:2334:4556:6778:899A:ABBC:CDDE:EFF0
#endif
