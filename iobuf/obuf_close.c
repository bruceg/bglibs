#include "iobuf.h"

int obuf_close(obuf* out)
{
  if (!obuf_flush(out)) return 0;
  return iobuf_close(&out->io);
}
