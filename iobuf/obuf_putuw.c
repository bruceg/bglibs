#include "iobuf.h"

static int obuf_putuw_rec(obuf* out, unsigned long data, unsigned width)
{
  if (width) --width;
  if (data >= 10) {
    if (!obuf_putuw_rec(out, data/10, width)) return 0;
  }
  else {
    if (!obuf_pad(out, width, ' ')) return 0;
  }
  return obuf_putc(out, (data % 10) + '0');
}

int obuf_putuw(obuf* out, unsigned long data, unsigned width)
{
  if (data < 10) {
    if (width && !obuf_pad(out, --width, ' ')) return 0;
    return obuf_putc(out, data + '0');
  }
  else
    return obuf_putuw_rec(out, data, width);
}
