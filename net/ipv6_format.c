#include <string.h>

#include "ipv6.h"
#include "ipv4.h"

static char hex[16] = "0123456789abcdef";

static char* format_part(uint16 i, char* s)
{
  if (i > 0xfff) *s++ = hex[(i >> 12) & 0xf];
  if (i > 0xff)  *s++ = hex[(i >> 8) & 0xf];
  if (i > 0xf)   *s++ = hex[(i >> 4) & 0xf];
  *s++ = hex[i & 0xf];
  return s;
}

static unsigned format_ipv4(char* buf, const ipv6addr* addr)
{
  ipv4addr a4;
  memcpy(&a4.addr, &addr->addr[12], 4);
  return fmt_ipv4addr(buf, &a4);
}

/** Produce a formatted C string from an IPv6 address.

\note The return value is statically allocated.  Multiple calls to this
function will return pointers to the same string.
*/
const char* ipv6_format(const ipv6addr* addr)
{
  static char buf[40];
  buf[fmt_ipv6addr(buf, addr)] = 0;
  return buf;
}

/** Produce a formatted string from an IPv6 address.

The given buffer must be at least 39 bytes long, or 40 bytes if it needs
to contain the standard trailing \c NUL byte.

\return The number of bytes written to the buffer.

\note This routine is thread and recursion safe.
*/
unsigned fmt_ipv6addr(char* buffer, const ipv6addr* addr)
{
  uint16 bits[8];
  int i;
  int first;
  char* s = buffer;

  /* Convert raw address to array of 16-bit parts */
  for (i = 0; i < 8; ++i)
    bits[i] = addr->addr[i*2] << 8 | addr->addr[i*2+1];
  for (first = 0; first < 8; ++first)
    if (bits[first] != 0)
      break;
  if (first >= 1) {
    *s++ = ':';
    *s++ = ':';
    /* A couple of special cases, to simplify IPv4 style output */
    if (first >= 8)
      i = 1;
    else if (first == 7 && bits[7] == 1)
      *s++ = '1';
    else if (first >= 6)
      s += format_ipv4(s, addr);
    else if (first == 5 && bits[5] == 0xffff) {
      memcpy(s, "ffff:", 5);
      s += 5;
      s += format_ipv4(s, addr);
    }
    else
      for (i = first; i < 8; ++i) {
	if (i > first)
	  *s++ = ':';
	s = format_part(bits[i], s);
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
	*s++ = ':';
      s = format_part(bits[i], s);
    }
    if (zs < 8) {
      *s++ = ':';
      *s++ = ':';
      for (i = ze; i < 8; ++i) {
	if (i > ze)
	  *s++ = ':';
	s = format_part(bits[i], s);
      }
    }
  }
  return s - buffer;
}

#ifdef SELFTEST_MAIN
static void test(const char a[16])
{
  ipv6addr ip;
  int i;
  for (i = 0; i < 16; ++i)
    obuf_putxw(&outbuf, (unsigned char)a[i], 2, '0');
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
00000000000000000000ffff011464c8=::ffff:1.20.100.200
00000000010100000000000000000000=::101:0:0:0:0:0
108000000000000000080800200c417a=1080::8:800:200c:417a
ff010000000000000000000000000101=ff01::101
ff010000000000000000000000000000=ff01::
0112233445566778899aabbccddeeff0=112:2334:4556:6778:899a:abbc:cdde:eff0
#endif
