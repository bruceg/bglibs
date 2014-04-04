#include "ipv6.h"
#include "ipv4.h"

static int hex2bin(char c)
{
  switch (c) {
  case '0':           return 0x0;
  case '1':           return 0x1;
  case '2':           return 0x2;
  case '3':           return 0x3;
  case '4':           return 0x4;
  case '5':           return 0x5;
  case '6':           return 0x6;
  case '7':           return 0x7;
  case '8':           return 0x8;
  case '9':           return 0x9;
  case 'A': case 'a': return 0xa;
  case 'B': case 'b': return 0xb;
  case 'C': case 'c': return 0xc;
  case 'D': case 'd': return 0xd;
  case 'E': case 'e': return 0xe;
  case 'F': case 'f': return 0xf;
  default: return -1;
  }
}

static const char* parse_hexpart(const char* s, uint16* num)
{
  int i = 0;
  int t;
  while ((t = hex2bin(*s)) != -1) {
    ++s;
    i = (i << 4) | t;
    if (i > 0xffff)
      return 0;
  }
  *num = i;
  return s;
}

static void set(ipv6addr* addr, int offset, uint16 bit)
{
  offset *= 2;
  addr->addr[offset++] = bit >> 8;
  addr->addr[offset] = bit;
}

/** Scan a C string for an IPv6 address.

\return \c NULL if parsing failed, otherwise a pointer to the
first character after the end of the address.
*/
const char* ipv6_scan(const char* s, ipv6addr* addr)
{
  uint16 bits[8];
  unsigned len1;
  unsigned len2;
  unsigned i;
  len1 = len2 = 0;
  if (s[0] == ':' && s[1] == ':')
    ++s;
  else {
    while (len1 < 8) {
      const char* news;
      if ((news = parse_hexpart(s, &bits[len1])) == 0 ||
	  (news == s && *news != ':'))
	return 0;
      s = news + (*news == ':');
      ++len1;
      if (*s == ':')
	break;
    }
    for (i = 0; i < len1; ++i)
      set(addr, i, bits[i]);
  }
  if (len1 < 8) {
    ++s;
    while (len2 < 8-len1) {
      const char* news;
      if ((news = parse_hexpart(s, &bits[len2])) == 0)
	return 0;
      if (news == s) break;
      s = news;
      ++len2;
      if (*s != ':') break;
      ++s;
    }
    for (i = len1; i < 8-len2; ++i)
      set(addr, i, 0);
    for (i = 0; i < len2; ++i)
      set(addr, i+8-len2, bits[i]);
  }
  /* handle IPv4 convenience addresses */
  if (len1+len2 <= 6 && *s == '.') {
    ipv4addr i4;
    while (*--s != ':') ;
    ++s;
    --len2;
    for (i = len1; i < 6-len2; ++i)
      set(addr, i, 0);
    for (i = 0; i < len2; ++i)
      set(addr, i+6-len2, bits[i]);
    // Parse IPv4 address here
    if ((s = ipv4_scan(s, &i4)) == 0)
      return 0;
    addr->addr[12] = i4.addr[0];
    addr->addr[13] = i4.addr[1];
    addr->addr[14] = i4.addr[2];
    addr->addr[15] = i4.addr[3];
  }
  return s;
}

#ifdef SELFTEST_MAIN
static void test(const char* start)
{
  ipv6addr ip;
  int i;
  const char* end;
  obuf_put2s(&outbuf, start, "=");
  end = ipv6_scan(start, &ip);
  if (end == 0)
    obuf_puts(&outbuf, "NULL");
  else {
    for (i = 0; i < 16; ++i) {
      obuf_putxw(&outbuf, ip.addr[i], 2, '0');
    }
    obuf_putc(&outbuf, '+');
    if (*end != 0)
      obuf_puts(&outbuf, end);
  }
  NL();
  obuf_flush(&outbuf);
}

MAIN
{
  test("::");
  test("::1");
  test("::0.0.0.2");
  test("::FFFF:1.20.100.200");
  test("::FFFF:1.20.100.200x");
  test("::101:0:0:0:0:0");
  test("1080::8:800:200C:417A");
  test("FF01::101");
  test("FF01::");
  test("FF01::x");
  test("112:2334:4556:6778:899A:ABBC:CDDE:EFF0");
  test("112:2334:4556:6778:899A:ABBC:CDDE:EFF0x");
}
#endif
#ifdef SELFTEST_EXP
::=00000000000000000000000000000000+
::1=00000000000000000000000000000001+
::0.0.0.2=00000000000000000000000000000002+
::FFFF:1.20.100.200=00000000000000000000ffff011464c8+
::FFFF:1.20.100.200x=00000000000000000000ffff011464c8+x
::101:0:0:0:0:0=00000000010100000000000000000000+
1080::8:800:200C:417A=108000000000000000080800200c417a+
FF01::101=ff010000000000000000000000000101+
FF01::=ff010000000000000000000000000000+
FF01::x=ff010000000000000000000000000000+x
112:2334:4556:6778:899A:ABBC:CDDE:EFF0=0112233445566778899aabbccddeeff0+
112:2334:4556:6778:899A:ABBC:CDDE:EFF0x=0112233445566778899aabbccddeeff0+x
#endif
