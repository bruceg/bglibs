#include "ipv4.h"

static unsigned char parse_part(const char* str, const char** end)
{
  unsigned char i = 0;
  if (*str < '0' || *str > '9')
    *end = 0;
  else {
    do {
      unsigned char newi;
      newi = i * 10 + *str - '0';
      if (newi < i || newi > 255) { *end = 0; return 0; }
      ++str;
      i = newi;
    } while (*str >= '0' && *str <= '9');
    *end = str;
  }
  return i;
}

int ipv4_parse(const char* str, ipv4addr* addr, const char** end)
{
  addr->addr[0] = parse_part(str, end); if (*end == str) return 0;
  if (**end != '.' && addr->addr[0] == 0) {
    addr->addr[1] = addr->addr[2] = addr->addr[3] = 0;
    return 1;
  }
  str = *end + 1; addr->addr[1] = parse_part(str, end);
  if (*end == str || **end != '.') return 0;
  str = *end + 1; addr->addr[2] = parse_part(str, end);
  if (*end == str || **end != '.') return 0;
  str = *end + 1; addr->addr[3] = parse_part(str, end);
  if (*end == str) return 0;
  return 1;
}
