#include "iobuf.h"

int obuf_pad(obuf* out, unsigned width, char ch)
{
  while (width) {
    if (!obuf_putc(out, ch)) return 0;
    --width;
  }
  return 1;
}
