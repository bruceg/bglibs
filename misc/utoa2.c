#include "fmt.h"

char* utoa2(unsigned long i, char* buf)
{
  if (i < 10)
    *buf = i + '0';
  else {
    buf = utoa2(i / 10, buf);
    *buf = (i % 10) + '0';
  }
  *++buf = 0;
  return buf;
}

#ifdef SELFTEST_MAIN
void test(unsigned long u)
{
  char buf[32];
  char* ptr;
  ptr = utoa2(u, buf);
  obuf_write(&outbuf, buf, ptr - buf);
  NL();
}

MAIN
{
  test(0);
  test(1);
  test(11);
  test(0x7fffffffUL);
  test(0x80000000UL);
  test(0xffffffffUL);
}
#endif
#ifdef SELFTEST_EXP
0
1
11
2147483647
2147483648
4294967295
#endif
