#include "iobuf.h"

int obuf_sign_pad(obuf* out, int sign, unsigned width, char pad)
{
  if (!width) return !sign || obuf_putc(out, '-');
  if (pad != '0' && !obuf_pad(out, width, pad)) return 0;
  if (sign && !obuf_putc(out, '-')) return 0;
  if (pad == '0' && !obuf_pad(out, width, pad)) return 0;
  return 1;
}

static int obuf_putiwll_rec(obuf* out, unsigned long long data, int sign,
			    unsigned width, char pad)
{
  if (width) --width;
  if (data >= 10) {
    if (!obuf_putiwll_rec(out, data/10, sign, width, pad)) return 0;
  }
  else {
    if (!obuf_sign_pad(out, sign, width, pad)) return 0;
  }
  return obuf_putc(out, (data % 10) + '0');
}

/** Write a signed long long integer to the \c obuf with optional
    padding. */
int obuf_putiwll(obuf* out, long long data, unsigned width, char pad)
{
  int sign;

  sign = 0;
  if (data < 0) {
    sign = 1;
    data = -data;
    if (width) -- width;
  }
  if (data < 10) {
    if (width && !obuf_sign_pad(out, sign, width-1, pad)) return 0;
    return obuf_putc(out, data + '0');
  }
  return obuf_putiwll_rec(out, data, sign, width, pad);
}
