#include <errno.h>
#include <unistd.h>
#include "iobuf.h"

int obuf_flush(obuf* out)
{
  iobuf* io;
  unsigned wr;
  
  io = &out->io;
  if (io->flags) return 0;
  while (io->bufstart < io->buflen) {
    if (io->timeout && !iobuf_timeout(io, 1)) return 0;
    wr = write(io->fd, io->buffer+io->bufstart, io->buflen-io->bufstart);
    if (wr == (unsigned)-1) IOBUF_SET_ERROR(io);
    io->bufstart += wr;
    io->offset += wr;
  }
  io->buflen = 0;
  io->bufstart = 0;
  out->bufpos = 0;
  return 1;
}
