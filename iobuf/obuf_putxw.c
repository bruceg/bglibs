#include "iobuf.h"

static char bin2hex[16] = "0123456789abcdef";

static int obuf_putxw_rec(obuf* out, unsigned long data,
			  unsigned width, char pad)
{
  if (width) --width;
  if (data >= 16) {
    if (!obuf_putxw_rec(out, data/16, width, pad)) return 0;
  }
  else {
    if (!obuf_pad(out, width, pad)) return 0;
  }
  return obuf_putc(out, bin2hex[data % 16]);
}

/** Write an unsigned integer to the \c obuf as hexadecimal with
    optional padding. */
int obuf_putxw(obuf* out, unsigned long data, unsigned width, char pad)
{
  if (data < 16) {
    if (width && !obuf_pad(out, --width, pad)) return 0;
    return obuf_putc(out, bin2hex[data]);
  }
  return obuf_putxw_rec(out, data, width, pad);
}
