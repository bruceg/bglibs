#include "iobuf.h"

/** Look up the current effective read position. */
unsigned ibuf_tell(ibuf* in)
{
  iobuf* io;
  io = &(in->io);
  return io->offset - io->buflen + io->bufstart;
}
