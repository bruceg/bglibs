#include "iobuf.h"

static int obuf_putunumw_rec(obuf* out, unsigned long data,
			     unsigned width, char pad,
			     unsigned base, const char* digits)
{
  if (width) --width;
  if (data >= base) {
    if (!obuf_putunumw_rec(out, data/base, width, pad, base, digits)) return 0;
  }
  else {
    if (!obuf_pad(out, width, pad)) return 0;
  }
  return obuf_putc(out, digits[data % base]);
}

/** Write an unsigned integer to the \c obuf with optional padding. */
int obuf_putunumw(obuf* out, unsigned long data, unsigned width, char pad,
		  unsigned base, const char* digits)
{
  if (data < base) {
    if (width && !obuf_pad(out, --width, pad)) return 0;
    return obuf_putc(out, digits[data]);
  }
  return obuf_putunumw_rec(out, data, width, pad, base, digits);
}

static char dec_digits[10] = "0123456789";

int obuf_putuw(obuf* out, unsigned long data, unsigned width, char pad)
{
  return obuf_putunumw(out, data, width, pad, 10, dec_digits);
}

int obuf_putu(obuf* out, unsigned long data)
{
  return obuf_putunumw(out, data, 0, 0, 10, dec_digits);
}

static char hex_digits[16] = "0123456789abcdef";

int obuf_putxw(obuf* out, unsigned long data, unsigned width, char pad)
{
  return obuf_putunumw(out, data, width, pad, 10, hex_digits);
}

int obuf_putx(obuf* out, unsigned long data)
{
  return obuf_putunumw(out, data, 0, 0, 10, hex_digits);
}

static char hex_udigits[16] = "0123456789ABCDEF";

int obuf_putXw(obuf* out, unsigned long data, unsigned width, char pad)
{
  return obuf_putunumw(out, data, width, pad, 10, hex_udigits);
}

int obuf_putX(obuf* out, unsigned long data)
{
  return obuf_putunumw(out, data, 0, 0, 10, hex_udigits);
}
