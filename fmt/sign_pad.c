#include <string.h>
#include "number.h"

/** Format padding for a signed number. */
unsigned fmt_sign_pad(char* buffer, int sign, unsigned width, char pad)
{
  char* s = buffer;
  if (buffer == 0)
    return width + (sign != 0);
  if (!width) {
    if (sign)
      *s++ = '-';
  }
  else {
    if (pad != '0') {
      memset(s, pad, width);
      s += width;
    }
    if (sign)
      *s++ = '-';
    if (pad == '0') {
      memset(s, pad, width);
      s += width;
    }
  }
  return s - buffer;
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
void test(int sign, unsigned width, char pad)
{
  char buf[10];
  buf[fmt_sign_pad(buf, sign, width, pad)] = 0;
  obuf_puts(&outbuf, buf);
  NL();
}

MAIN
{
  test(0, 0,   0);
  test(1, 0,   0);
  test(0, 4, 'x');
  test(1, 4, 'x');
  test(0, 4, '0');
  test(1, 4, '0');
}
#endif
#ifdef SELFTEST_EXP

-
xxxx
xxxx-
0000
-0000
#endif
