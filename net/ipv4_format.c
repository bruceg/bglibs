#include "ipv4.h"

static char* format_part(unsigned char i, char* str)
{
  if (i >= 10) {
    str = format_part(i / 10, str);
    i = i % 10;
  }
  *str++ = i + '0';
  return str;
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

/** Produce a formatted string from an IPv4 address.

The given buffer must be at least 15 bytes long, or 16 bytes if it needs
to contain the standard trailing \c NUL byte.

\return The number of bytes written to the buffer.

\note This routine is thread and recursion safe.
*/
unsigned fmt_ipv4addr(char* buffer, const ipv4addr* addr)
{
  char* str = buffer;
  str = format_part(addr->addr[0], str); *str++ = '.';
  str = format_part(addr->addr[1], str); *str++ = '.';
  str = format_part(addr->addr[2], str); *str++ = '.';
  str = format_part(addr->addr[3], str);
  return str - buffer;
}
