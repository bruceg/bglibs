#include "iobuf.h"

int obuf_sign_pad(obuf* out, int sign, unsigned width, char pad)
{
  if (!width) return !sign || obuf_putc(out, '-');
  if (pad != '0' && !obuf_pad(out, width, pad)) return 0;
  if (sign && !obuf_putc(out, '-')) return 0;
  if (pad == '0' && !obuf_pad(out, width, pad)) return 0;
  return 1;
}

static int obuf_putsnumw_rec(obuf* out, long long data, int sign,
			     unsigned width, char pad,
			     unsigned base, const char* digits)
{
  if (width) --width;
  if (data >= base) {
    if (!obuf_putsnumw_rec(out, data/base, sign, width, pad, base, digits))
      return 0;
  }
  else {
    if (!obuf_sign_pad(out, sign, width, pad)) return 0;
  }
  return obuf_putc(out, (data % base) + '0');
}

/** Write a signed integer to the \c obuf with optional padding. */
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
  if (data < base) {
    if (width) {
      if (!obuf_sign_pad(out, sign, width-1, pad)) return 0;
    }
    else if (sign)
      if (!obuf_putc(out, '-')) return 0;
    return obuf_putc(out, data + '0');
  }
  return obuf_putsnumw_rec(out, data, sign, width, pad, base, digits);
}

static char dec_digits[10] = "0123456789";

int obuf_putiwll(obuf* out, long long data, unsigned width, char pad)
{
  return obuf_putsnumw(out, data, width, pad, 10, dec_digits);
}

int obuf_putill(obuf* out, long long data)
{
  return obuf_putsnumw(out, data, 0, 0, 10, dec_digits);
}
