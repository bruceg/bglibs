#include "iobuf.h"

int obuf_putnetstring(obuf* out, const char* data, unsigned datalen)
{
  if (!obuf_putu(out, datalen)) return 0;
  if (!obuf_putc(out, ':')) return 0;
  if (!obuf_write(out, data, datalen)) return 0;
  if (!obuf_putc(out, ',')) return 0;
  return 1;
}
