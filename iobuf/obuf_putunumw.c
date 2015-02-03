#include "fmt.h"
#include "obuf.h"

/** Write an unsigned integer to the \c obuf with optional padding. */
int obuf_putunumw(obuf* out, unsigned long data, unsigned width, char pad,
		  unsigned base, const char* digits)
{
  unsigned len = fmt_unumw(0, data, width, pad, base, digits);
  char buf[len];
  fmt_unumw(buf, data, width, pad, base, digits);
  return obuf_write(out, buf, len);
}

/** Write an unsigned integer as decimal to the \c obuf with padding. */
int obuf_putuw(obuf* out, unsigned long data, unsigned width, char pad)
{
  return obuf_putunumw(out, data, width, pad, 10, obuf_dec_digits);
}

/** Write an unsigned integer as decimal to the \c obuf. */
int obuf_putu(obuf* out, unsigned long data)
{
  return obuf_putunumw(out, data, 0, 0, 10, obuf_dec_digits);
}

/** Write an unsigned integer as (lower-case) hexadecimal to the \c obuf
    with padding. */
int obuf_putxw(obuf* out, unsigned long data, unsigned width, char pad)
{
  return obuf_putunumw(out, data, width, pad, 16, obuf_hex_lcase_digits);
}

/** Write an unsigned integer as (lower-case) hexadecimal to the \c obuf. */
int obuf_putx(obuf* out, unsigned long data)
{
  return obuf_putunumw(out, data, 0, 0, 16, obuf_hex_lcase_digits);
}

/** Write an unsigned integer as (upper-case) hexadecimal to the \c obuf
    with padding. */
int obuf_putXw(obuf* out, unsigned long data, unsigned width, char pad)
{
  return obuf_putunumw(out, data, width, pad, 16, obuf_hex_ucase_digits);
}

/** Write an unsigned integer as (upper-case) hexadecimal to the \c obuf. */
int obuf_putX(obuf* out, unsigned long data)
{
  return obuf_putunumw(out, data, 0, 0, 16, obuf_hex_ucase_digits);
}

#ifdef SELFTEST_MAIN
MAIN
{
  obuf_putuw(&outbuf,  10, 0,   0); NL();
  obuf_putuw(&outbuf,  10, 5, ' '); NL();
  obuf_putuw(&outbuf,  10, 5, '0'); NL();
  obuf_putxw(&outbuf,  30, 0,   0); NL();
  obuf_putxw(&outbuf,  30, 5, ' '); NL();
  obuf_putxw(&outbuf,  30, 5, '0'); NL();
  obuf_putXw(&outbuf,  30, 0,   0); NL();
  obuf_putXw(&outbuf,  30, 5, ' '); NL();
  obuf_putXw(&outbuf,  30, 5, '0'); NL();
}
#endif
#ifdef SELFTEST_EXP
10
   10
00010
1e
   1e
0001e
1E
   1E
0001E
#endif
