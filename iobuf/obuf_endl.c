#include "iobuf.h"

int obuf_endl(obuf* out)
{
  return obuf_write(out, "\n", 1) && obuf_flush(out);
}
