#include "iobuf.h"

int ibuf_eof(ibuf* in)
{
  return (in->io.flags & IOBUF_EOF) &&
    (in->io.bufstart >= in->io.buflen);
}
