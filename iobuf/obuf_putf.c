#include <stdarg.h>
#include <string.h>
#include "obuf.h"
#include "fmt/multi.h"

/** Write a formatted string to the \c obuf using \c fmt_multi */
int obuf_putf(obuf* out, const char* format, ...)
{
  unsigned length;
  int i;
  va_list ap;

  va_start(ap, format);
  length = fmt_multiv(0, format, ap);
  va_end(ap);

  va_start(ap, format);
  {
    char buf[length];
    fmt_multiv(buf, format, ap);
    i = obuf_write(out, buf, length);
  }
  va_end(ap);

  return i;
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
MAIN
{
  obuf_putf(&outbuf, "d{bar}x", 10, 20); NL();
}
#endif
#ifdef SELFTEST_EXP
10bar14
#endif
