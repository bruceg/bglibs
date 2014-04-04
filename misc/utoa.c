#include "misc.h"

static char utoa_buf[32];

const char* utoa(unsigned long i)
{
  char* ptr = utoa_buf + sizeof utoa_buf - 1;
  *ptr-- = 0;
  while (i >= 10) {
    *ptr-- = (i % 10) + '0';
    i /= 10;
  }
  *ptr = i + '0';
  return ptr;
}

#ifdef SELFTEST_MAIN
void test(unsigned long u)
{
  obuf_putf(&outbuf, "{utoa(}lu{) = '}s{'}", u, utoa(u));
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
utoa(0) = '0'
utoa(1) = '1'
utoa(11) = '11'
utoa(2147483647) = '2147483647'
utoa(2147483648) = '2147483648'
utoa(4294967295) = '4294967295'
#endif
