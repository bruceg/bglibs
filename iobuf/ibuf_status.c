#include "iobuf.h"

/** Test if the \c ibuf has reaced end of file. */
int ibuf_eof(ibuf* in)
{
  return (in->io.flags & IOBUF_EOF) &&
    (in->io.bufstart >= in->io.buflen);
}
