#include "iobuf.h"

static int obuf_putuwll_rec(obuf* out, unsigned long long data,
			    unsigned width, char pad)
{
  if (width) --width;
  if (data >= 10) {
    if (!obuf_putuwll_rec(out, data/10, width, pad)) return 0;
  }
  else {
    if (!obuf_pad(out, width, pad)) return 0;
  }
  return obuf_putc(out, (data % 10) + '0');
}

int obuf_putuwll(obuf* out, unsigned long long data, unsigned width, char pad)
{
  if (data < 10) {
    if (width && !obuf_pad(out, --width, pad)) return 0;
    return obuf_putc(out, data + '0');
  }
  return obuf_putuwll_rec(out, data, width, pad);
}
