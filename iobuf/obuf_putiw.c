#include "iobuf.h"

int obuf_putiw(obuf* out, long data, unsigned width)
{
  if (data < 0) {
    if (!obuf_putc(out, '-')) return 0;
    data = -data;
    if (width) --width;
  }
  return obuf_putuw(out, (unsigned long)data, width);
}
