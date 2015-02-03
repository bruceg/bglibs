#include "fmt.h"
#include "obuf.h"

/** Write an unsigned long long integer to the \c obuf with optional
    padding. */
int obuf_putullnumw(obuf* out, unsigned long long data,
		    unsigned width, char pad,
		    unsigned base, const char* digits)
{
  unsigned len = fmt_ullnumw(0, data, width, pad, base, digits);
  char buf[len];
  fmt_ullnumw(buf, data, width, pad, base, digits);
  return obuf_write(out, buf, len);
}

/** Write an unsigned long long integer as decimal to the \c obuf with
    padding. */
int obuf_putuwll(obuf* out, unsigned long long data, unsigned width, char pad)
{
  return obuf_putullnumw(out, data, width, pad, 10, fmt_lcase_digits);
}

/** Write an unsigned long long integer as decimal to the \c obuf. */
int obuf_putull(obuf* out, unsigned long long data)
{
  return obuf_putullnumw(out, data, 0, 0, 10, fmt_lcase_digits);
}

/** Write an unsigned long long integer as (lower-case) hexadecimal to
    the \c obuf with padding. */
int obuf_putxwll(obuf* out, unsigned long long data, unsigned width, char pad)
{
  return obuf_putullnumw(out, data, width, pad, 16, fmt_lcase_digits);
}

/** Write an unsigned long long integer as (lower-case) hexadecimal to
    the \c obuf. */
int obuf_putxll(obuf* out, unsigned long long data)
{
  return obuf_putullnumw(out, data, 0, 0, 16, fmt_lcase_digits);
}

/** Write an unsigned long long integer as (upper-case) hexadecimal to
    the \c obuf with padding. */
int obuf_putXwll(obuf* out, unsigned long long data, unsigned width, char pad)
{
  return obuf_putullnumw(out, data, width, pad, 16, fmt_ucase_digits);
}

/** Write an unsigned long long integer as (upper-case) hexadecimal to
    the \c obuf. */
int obuf_putXll(obuf* out, unsigned long long data)
{
  return obuf_putullnumw(out, data, 0, 0, 16, fmt_ucase_digits);
}

#ifdef SELFTEST_MAIN
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
