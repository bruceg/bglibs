#include "iobuf.h"

/** Pad the output with \c width instances of the single character \c ch. */
int obuf_pad(obuf* out, unsigned width, char ch)
{
  while (width) {
    if (!obuf_putc(out, ch)) return 0;
    --width;
  }
  return 1;
}
