#include "fmt.h"

unsigned long strtou(const char* s, const char** end)
{
  unsigned long u;
  unsigned digit;
  for (u = 0; (digit = *s - '0') <= 9; ++s)
    u = u * 10 + digit;
  *end = s;
  return u;
}

#ifdef SELFTEST_MAIN
void test(const char* s)
{
  const char* end;
  unsigned long u;
  u = strtou(s, &end);
  obuf_putf(&outbuf, "{u=}lu{ end='}s{'}", u, end);
  NL();
}

MAIN
{
  test("");
  test("0");
  test("1");
  test("1111f");
  test("-12");
  test("f");
}
#endif
#ifdef SELFTEST_EXP
u=0 end=''
u=0 end=''
u=1 end=''
u=1111 end='f'
u=0 end='-12'
u=0 end='f'
#endif
