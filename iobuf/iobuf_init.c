#include <stdlib.h>
#include "iobuf.h"

unsigned iobuf_bufsize = 4096;

int iobuf_init(iobuf* io, int fd, int do_close, unsigned bufsize, char* buffer)
{
  if (!bufsize) bufsize = iobuf_bufsize;
  if (!buffer) {
    if ((buffer = malloc(bufsize)) == 0) return 0;
    io->do_free = 1;
  }
  else
    io->do_free = 0;
  io->fd = fd;
  io->buffer = buffer;
  io->bufsize = bufsize;
  io->buflen = 0;
  io->bufstart = 0;
  io->offset = 0;
  io->errnum = 0;
  io->flags = 0;
  io->do_close = do_close;
  return 1;
}
