#include <string.h>
#include "obuf.h"
#include "fmt/multi.h"

/** Write a formatted string using \c fmt_multi from a \c va_list */
int obuf_putfv(obuf* out, const char* format, va_list ap)
{
  unsigned length;
  int i;
  va_list ap2;

  va_copy(ap2, ap);
  length = fmt_multiv(0, format, ap2);
  va_end(ap2);

  {
    char buf[length];
    fmt_multiv(buf, format, ap);
    i = obuf_write(out, buf, length);
  }

  return i;
}

/** Write a formatted string using \c fmt_multi from variable arguments */
int obuf_putf(obuf* out, const char* format, ...)
{
  int i;
  va_list ap;
  va_start(ap, format);
  i = obuf_putfv(out, format, ap);
  va_end(ap);
  return i;
}

#ifdef SELFTEST_MAIN
MAIN
{
  obuf_putf(&outbuf, "d{bar}x", 10, 20); NL();
}
#endif
#ifdef SELFTEST_EXP
10bar14
#endif
