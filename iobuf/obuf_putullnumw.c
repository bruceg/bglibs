/* $Id$ */
#include "obuf.h"

static int obuf_putullnumw_rec(obuf* out, unsigned long long data,
			       unsigned width, char pad,
			       unsigned base, const char* digits)
{
  if (width) --width;
  if (data >= base) {
    if (!obuf_putullnumw_rec(out, data/base, width, pad, base, digits))
      return 0;
  }
  else {
    if (!obuf_pad(out, width, pad)) return 0;
  }
  return obuf_putc(out, digits[data % base]);
}

/** Write an unsigned long long integer to the \c obuf with optional
    padding. */
int obuf_putullnumw(obuf* out, unsigned long long data,
		    unsigned width, char pad,
		    unsigned base, const char* digits)
{
  if (data < base) {
    if (width && !obuf_pad(out, --width, pad)) return 0;
    return obuf_putc(out, digits[data]);
  }
  return obuf_putullnumw_rec(out, data, width, pad, base, digits);
}

/** Write an unsigned long long integer as decimal to the \c obuf with
    padding. */
int obuf_putuwll(obuf* out, unsigned long long data, unsigned width, char pad)
{
  return obuf_putullnumw(out, data, width, pad, 10, obuf_dec_digits);
}

/** Write an unsigned long long integer as decimal to the \c obuf. */
int obuf_putull(obuf* out, unsigned long long data)
{
  return obuf_putullnumw(out, data, 0, 0, 10, obuf_dec_digits);
}

/** Write an unsigned long long integer as (lower-case) hexadecimal to
    the \c obuf with padding. */
int obuf_putxwll(obuf* out, unsigned long long data, unsigned width, char pad)
{
  return obuf_putullnumw(out, data, width, pad, 16, obuf_hex_lcase_digits);
}

/** Write an unsigned long long integer as (lower-case) hexadecimal to
    the \c obuf. */
int obuf_putxll(obuf* out, unsigned long long data)
{
  return obuf_putullnumw(out, data, 0, 0, 16, obuf_hex_lcase_digits);
}

/** Write an unsigned long long integer as (upper-case) hexadecimal to
    the \c obuf with padding. */
int obuf_putXwll(obuf* out, unsigned long long data, unsigned width, char pad)
{
  return obuf_putullnumw(out, data, width, pad, 16, obuf_hex_ucase_digits);
}

/** Write an unsigned long long integer as (upper-case) hexadecimal to
    the \c obuf. */
int obuf_putXll(obuf* out, unsigned long long data)
{
  return obuf_putullnumw(out, data, 0, 0, 16, obuf_hex_ucase_digits);
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
MAIN
{
  obuf_putuwll(&outbuf,  10, 0,   0); NL();
  obuf_putuwll(&outbuf,  10, 5, ' '); NL();
  obuf_putuwll(&outbuf,  10, 5, '0'); NL();
  obuf_putuwll(&outbuf,  1000000000000ULL, 0,    0); NL();
  obuf_putuwll(&outbuf,  1000000000000ULL, 15, ' '); NL();
  obuf_putuwll(&outbuf,  1000000000000ULL, 15, '0'); NL();
  obuf_putxwll(&outbuf,  1000000000000ULL, 0,    0); NL();
  obuf_putxwll(&outbuf,  1000000000000ULL, 15, ' '); NL();
  obuf_putxwll(&outbuf,  1000000000000ULL, 15, '0'); NL();
  obuf_putXwll(&outbuf,  1000000000000ULL, 0,    0); NL();
  obuf_putXwll(&outbuf,  1000000000000ULL, 15, ' '); NL();
  obuf_putXwll(&outbuf,  1000000000000ULL, 15, '0'); NL();
}
#endif
#ifdef SELFTEST_EXP
10
   10
00010
1000000000000
  1000000000000
001000000000000
e8d4a51000
     e8d4a51000
00000e8d4a51000
E8D4A51000
     E8D4A51000
00000E8D4A51000
#endif
