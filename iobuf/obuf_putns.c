#include <stdarg.h>
#include <string.h>
#include "obuf.h"

/** Write N C strings to the \c obuf */
int obuf_putns(obuf* out, unsigned int count, ...)
{
  const char* ptr;
  va_list ap;
  va_start(ap, count);
  while (count-- > 0) {
    if ((ptr = va_arg(ap, const char*)) != 0)
      if (!obuf_puts(out, ptr))
	return 0;
  }
  va_end(ap);
  return 1;
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
MAIN
{
  obuf_putns(&outbuf, 4, "1", 0, "2", "3"); NL();
}
#endif
#ifdef SELFTEST_EXP
123
#endif
