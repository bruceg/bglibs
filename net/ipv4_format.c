#include "ipv4.h"

static char* format_part(unsigned char i, char* s)
{
  if (i >= 10) {
    s = format_part(i / 10, s);
    i = i % 10;
  }
  *s++ = i + '0';
  return s;
}

/** Produce a formatted C string from an IPv4 address.

\note The return value is statically allocated.  Multiple calls to this
function will return pointers to the same string.
*/
const char* ipv4_format(const ipv4addr* addr)
{
  static char buf[16];
  buf[fmt_ipv4addr(buf, addr)] = 0;
  return buf;
}

/** Produce a reverse-formatted C string from an IPv4 address.

\note The return value is statically allocated.  Multiple calls to this
function will return pointers to the same string.
*/
const char* ipv4_format_reverse(const ipv4addr* addr)
{
  static char buf[16];
  buf[fmt_ipv4addr_reverse(buf, addr)] = 0;
  return buf;
}

/** Produce a formatted string from an IPv4 address.

The given buffer must be at least 15 bytes long, or 16 bytes if it needs
to contain the standard trailing \c NUL byte.

\return The number of bytes written to the buffer.

\note This routine is thread and recursion safe.
*/
unsigned fmt_ipv4addr(char* buffer, const ipv4addr* addr)
{
  char* s = buffer;
  s = format_part(addr->addr[0], s); *s++ = '.';
  s = format_part(addr->addr[1], s); *s++ = '.';
  s = format_part(addr->addr[2], s); *s++ = '.';
  s = format_part(addr->addr[3], s);
  return s - buffer;
}

/** Produce a reverse-formatted string from an IPv4 address.

The given buffer must be at least 15 bytes long, or 16 bytes if it needs
to contain the standard trailing \c NUL byte.

\return The number of bytes written to the buffer.

\note This routine is thread and recursion safe.
*/
unsigned fmt_ipv4addr_reverse(char* buffer, const ipv4addr* addr)
{
  char* s = buffer;
  s = format_part(addr->addr[3], s); *s++ = '.';
  s = format_part(addr->addr[2], s); *s++ = '.';
  s = format_part(addr->addr[1], s); *s++ = '.';
  s = format_part(addr->addr[0], s);
  return s - buffer;
}

#ifdef SELFTEST_MAIN
void testfn(const ipv4addr* ip, unsigned (*fn)(char*,const ipv4addr*))
{
  char buffer[32];
  unsigned i;
  debugfn(i = fn(buffer, ip));
  buffer[i++] = '\n';
  buffer[i] = 0;
  obuf_putsflush(&outbuf, buffer);
}

void test(const ipv4addr* ip)
{
  testfn(ip, fmt_ipv4addr);
  testfn(ip, fmt_ipv4addr_reverse);
}

MAIN
{
  ipv4addr ip = {{1,2,3,4}};
  test(&IPV4ADDR_ANY);
  test(&IPV4ADDR_BROADCAST);
  test(&IPV4ADDR_LOOPBACK);
  test(&ip);
}
#endif
#ifdef SELFTEST_EXP
result=7
0.0.0.0
result=7
0.0.0.0
result=15
255.255.255.255
result=15
255.255.255.255
result=9
127.0.0.1
result=9
1.0.0.127
result=7
1.2.3.4
result=7
4.3.2.1
#endif
