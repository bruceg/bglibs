#include "obuf.h"

static int obuf_putsnumw_rec(obuf* out, long long data, int sign,
			     unsigned width, char pad,
			     unsigned base, const char* digits)
{
  if (width) --width;
  if (data >= (long)base) {
    if (!obuf_putsnumw_rec(out, data/base, sign, width, pad, base, digits))
      return 0;
  }
  else {
    if (!obuf_sign_pad(out, sign, width, pad)) return 0;
  }
  return obuf_putc(out, (data % base) + '0');
}

/** Write a signed long long integer to the \c obuf with optional padding. */
int obuf_putsllnumw(obuf* out, long long data, unsigned width, char pad,
		    unsigned base, const char* digits)
{
  int sign;

  sign = 0;
  if (data < 0) {
    sign = 1;
    data = -data;
    if (width) -- width;
  }
  if (data < (long)base) {
    if (width) {
      if (!obuf_sign_pad(out, sign, width-1, pad)) return 0;
    }
    else if (sign)
      if (!obuf_putc(out, '-')) return 0;
    return obuf_putc(out, data + '0');
  }
  return obuf_putsnumw_rec(out, data, sign, width, pad, base, digits);
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
