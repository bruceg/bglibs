#include <string.h>
#include "ipv4.h"

static const char* parse_part(const char* str, uint8* num)
{
  uint8 i = 0;
  if (*str < '0' || *str > '9')
    return 0;
  do {
    uint8 newi;
    newi = i * 10 + *str - '0';
    if (newi < i || newi > 255) return 0;
    ++str;
    i = newi;
  } while (*str >= '0' && *str <= '9');
  *num = i;
  return str;
}

const char* ipv4_scan(const char* start, ipv4addr* addr)
{
  const char* s;
  if ((s = parse_part(start, &addr->addr[0])) == 0) return 0;
  if (*s != '.' && addr->addr[0] == 0) {
    /* Special case -- The number "0" represents 0.0.0.0 */
    memset(addr, 0, sizeof *addr);
    return s;
  }
  if ((s = parse_part(s+1, &addr->addr[1])) == 0 && *s != '.') return 0;
  if ((s = parse_part(s+1, &addr->addr[2])) == 0 && *s != '.') return 0;
  if ((s = parse_part(s+1, &addr->addr[3])) == 0) return 0;
  return s;
}

int ipv4_parse(const char* start, ipv4addr* addr, const char** end)
{
  return (*end = ipv4_scan(start, addr)) != 0;
}
