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
    wr = write(io->fd, io->buffer+io->bufstart, io->buflen-io->bufstart);
    if (wr == -1) {
      io->flags |= IOBUF_ERROR;
      io->errnum = errno;
      return 0;
    }
    io->bufstart += wr;
    io->offset += wr;
  }
  io->buflen = 0;
  io->bufstart = 0;
  out->bufpos = 0;
  return 1;
}
