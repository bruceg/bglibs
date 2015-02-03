#include "fmt.h"
#include "obuf.h"

/** Write a signed long long integer to the \c obuf with optional padding. */
int obuf_putsllnumw(obuf* out, long long data, unsigned width, char pad,
		    unsigned base, const char* digits)
{
  unsigned len = fmt_sllnumw(0, data, width, pad, base, digits);
  char buf[len];
  fmt_sllnumw(buf, data, width, pad, base, digits);
  return obuf_write(out, buf, len);
}

/** Write a signed long long integer as decimal to the \c obuf with padding. */
int obuf_putiwll(obuf* out, long long data, unsigned width, char pad)
{
  return obuf_putsllnumw(out, data, width, pad, 10, obuf_dec_digits);
}

/** Write a signed long long integer as decimal to the \c obuf. */
int obuf_putill(obuf* out, long long data)
{
  return obuf_putsllnumw(out, data, 0, 0, 10, obuf_dec_digits);
}

#ifdef SELFTEST_MAIN
MAIN
{
  obuf_putiwll(&outbuf,  10, 0,   0); NL();
  obuf_putiwll(&outbuf, -10, 0,   0); NL();
  obuf_putiwll(&outbuf,  10, 5, ' '); NL();
  obuf_putiwll(&outbuf,  10, 5, '0'); NL();
  obuf_putiwll(&outbuf, -10, 5, ' '); NL();
  obuf_putiwll(&outbuf, -10, 5, '0'); NL();
  obuf_putiwll(&outbuf,  1000000000000LL, 0,    0); NL();
  obuf_putiwll(&outbuf, -1000000000000LL, 0,    0); NL();
  obuf_putiwll(&outbuf,  1000000000000LL, 15, ' '); NL();
  obuf_putiwll(&outbuf, -1000000000000LL, 15, ' '); NL();
  obuf_putiwll(&outbuf,  1000000000000LL, 15, '0'); NL();
  obuf_putiwll(&outbuf, -1000000000000LL, 15, '0'); NL();
}
#endif
#ifdef SELFTEST_EXP
10
-10
   10
00010
  -10
-0010
1000000000000
-1000000000000
  1000000000000
 -1000000000000
001000000000000
-01000000000000
#endif
