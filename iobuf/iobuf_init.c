#include <stdlib.h>
#include <string.h>
#include "iobuf.h"

unsigned iobuf_bufsize = 4096;

int iobuf_init(iobuf* io, int fd, int do_close, unsigned bufsize, char* buffer)
{
  memset(io, 0, sizeof *io);
  if (!bufsize) bufsize = iobuf_bufsize;
  if (!buffer) {
    if ((buffer = malloc(bufsize)) == 0) return 0;
    io->do_free = 1;
  }
  io->fd = fd;
  io->buffer = buffer;
  io->bufsize = bufsize;
  io->do_close = do_close;
  return 1;
}
