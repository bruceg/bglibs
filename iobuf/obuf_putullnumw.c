#include "iobuf.h"

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
  return obuf_putunumw(out, data, width, pad, 10, obuf_dec_digits);
}

/** Write an unsigned long long integer as decimal to the \c obuf. */
int obuf_putull(obuf* out, unsigned long long data)
{
  return obuf_putunumw(out, data, 0, 0, 10, obuf_dec_digits);
}

/** Write an unsigned long long integer as (lower-case) hexadecimal to
    the \c obuf with padding. */
int obuf_putxwll(obuf* out, unsigned long long data, unsigned width, char pad)
{
  return obuf_putunumw(out, data, width, pad, 10, obuf_hex_lcase_digits);
}

/** Write an unsigned long long integer as (lower-case) hexadecimal to
    the \c obuf. */
int obuf_putxll(obuf* out, unsigned long long data)
{
  return obuf_putunumw(out, data, 0, 0, 10, obuf_hex_lcase_digits);
}

/** Write an unsigned long long integer as (upper-case) hexadecimal to
    the \c obuf with padding. */
int obuf_putXwll(obuf* out, unsigned long long data, unsigned width, char pad)
{
  return obuf_putunumw(out, data, width, pad, 10, obuf_hex_ucase_digits);
}

/** Write an unsigned long long integer as (upper-case) hexadecimal to
    the \c obuf. */
int obuf_putXll(obuf* out, unsigned long long data)
{
  return obuf_putunumw(out, data, 0, 0, 10, obuf_hex_ucase_digits);
}
