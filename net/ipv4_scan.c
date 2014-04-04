#include <string.h>
#include "ipv4.h"

static const char* parse_part(const char* str, uint8* num)
{
  uint8 i = 0;
  if (*str < '0' || *str > '9')
    return 0;
  do {
    unsigned newi;
    newi = i * 10 + *str - '0';
    if (newi < i || newi > 255) return 0;
    ++str;
    i = newi;
  } while (*str >= '0' && *str <= '9');
  *num = i;
  return str;
}

/** Scan a C string for an IPv4 address.

\return \c NULL if parsing failed, otherwise a pointer to the
first character after the end of the address.
*/
const char* ipv4_scan(const char* start, ipv4addr* addr)
{
  const char* s;
  if ((s = parse_part(start, &addr->addr[0])) == 0) return 0;
  if (*s != '.' && addr->addr[0] == 0) {
    /* Special case -- The number "0" represents 0.0.0.0 */
    memset(addr, 0, sizeof *addr);
    return s;
  }
  if ((s = parse_part(s+1, &addr->addr[1])) == 0 || *s != '.') return 0;
  if ((s = parse_part(s+1, &addr->addr[2])) == 0 || *s != '.') return 0;
  if ((s = parse_part(s+1, &addr->addr[3])) == 0) return 0;
  return s;
}

/** Parse an IPv4 address.

\deprecated This function is a wrapper for \c ipv4_scan.

\return Non-zero (true) if parsing succeeded, and sets \c *end to the
first character past the end of the address.
*/
int ipv4_parse(const char* start, ipv4addr* addr, const char** end)
{
  return (*end = ipv4_scan(start, addr)) != 0;
}

#ifdef SELFTEST_MAIN
static void test(const char* start)
{
  ipv4addr ip;
  int i;
  const char* end;
  obuf_put2s(&outbuf, start, ": ");
  end = ipv4_scan(start, &ip);
  if (end == 0)
    obuf_puts(&outbuf, "NULL");
  else {
    for (i = 0; i < 4; ++i) {
      if (i > 0) obuf_putc(&outbuf, '.');
      obuf_puti(&outbuf, ip.addr[i]);
    }
    if (*end != 0)
      obuf_put2s(&outbuf, " + ", end);
  }
  NL();
  obuf_flush(&outbuf);
}

MAIN
{
  test("0");
  test("0.");
  test("0a");
  test("0.0.0.0");
  test("1.2.3.4");
  test("111.222.333.444");
  test("1.2.3.4a");
  test("1,2,3,4");
}
#endif
#ifdef SELFTEST_EXP
0: 0.0.0.0
0.: NULL
0a: 0.0.0.0 + a
0.0.0.0: 0.0.0.0
1.2.3.4: 1.2.3.4
111.222.333.444: NULL
1.2.3.4a: 1.2.3.4 + a
1,2,3,4: NULL
#endif
