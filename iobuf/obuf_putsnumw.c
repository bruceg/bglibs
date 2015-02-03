#include "fmt.h"
#include "obuf.h"

/** Write a signed integer to the \c obuf with optional padding. */
int obuf_putsnumw(obuf* out, long data, unsigned width, char pad,
		  unsigned base, const char* digits)
{
  unsigned len = fmt_snumw(0, data, width, pad, base, digits);
  char buf[len];
  fmt_snumw(buf, data, width, pad, base, digits);
  return obuf_write(out, buf, len);
}

/** Write a signed integer as decimal to the \c obuf with padding. */
int obuf_putiw(obuf* out, long data, unsigned width, char pad)
{
  return obuf_putsnumw(out, data, width, pad, 10, fmt_lcase_digits);
}

/** Write a signed integer as decimal to the \c obuf. */
int obuf_puti(obuf* out, long data)
{
  return obuf_putsnumw(out, data, 0, 0, 10, fmt_lcase_digits);
}

#ifdef SELFTEST_MAIN
MAIN
{
  obuf_putiw(&outbuf,  10, 0,   0); NL();
  obuf_putiw(&outbuf, -10, 0,   0); NL();
  obuf_putiw(&outbuf,  10, 5, ' '); NL();
  obuf_putiw(&outbuf,  10, 5, '0'); NL();
  obuf_putiw(&outbuf, -10, 5, ' '); NL();
  obuf_putiw(&outbuf, -10, 5, '0'); NL();
}
#endif
#ifdef SELFTEST_EXP
10
-10
   10
00010
  -10
-0010
#endif
