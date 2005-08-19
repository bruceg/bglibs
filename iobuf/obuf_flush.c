/* $Id$ */
#include <errno.h>
#include <unistd.h>
#include "obuf.h"

/** Write all pending data in the \c obuf to the file descriptor. */
int obuf_flush(obuf* out)
{
  iobuf* io;
  unsigned wr;
  
  io = &out->io;
  if (iobuf_bad(io)) return 0;
  while (io->bufstart < io->buflen) {
    if (io->timeout && !iobuf_timeout(io, 1)) return 0;
    wr = out->writefn(io->fd, io->buffer+io->bufstart,
		      io->buflen-io->bufstart);
    if (wr == (unsigned)-1) IOBUF_SET_ERROR(io);
    io->bufstart += wr;
    io->offset += wr;
  }
  io->buflen = 0;
  io->bufstart = 0;
  out->bufpos = 0;
  return 1;
}
