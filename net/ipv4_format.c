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

const char* ipv4_format(const ipv4addr addr)
{
  static char buf[16];
  char* str = buf;
  str = format_part(addr[0], str); *str++ = '.';
  str = format_part(addr[1], str); *str++ = '.';
  str = format_part(addr[2], str); *str++ = '.';
  str = format_part(addr[3], str); *str = 0;
  return buf;
}
