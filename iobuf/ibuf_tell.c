#include "iobuf.h"

unsigned ibuf_tell(ibuf* in)
{
  iobuf* io;
  io = &(in->io);
  return io->offset - io->buflen + io->bufstart;
}
