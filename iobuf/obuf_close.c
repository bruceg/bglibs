#include "iobuf.h"

int obuf_close(obuf* out)
{
  int result;
  result = obuf_flush(out);
  return iobuf_close(&out->io) && result;
}
