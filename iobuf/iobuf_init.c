#include <stdlib.h>
#include <string.h>
#include "iobuf.h"

unsigned iobuf_bufsize = 4096;

int iobuf_init(iobuf* io, int fd, unsigned bufsize, char* buffer, unsigned flags)
{
  memset(io, 0, sizeof *io);
  io->fd = fd;
  io->buffer = buffer;
  io->bufsize = bufsize;
  io->flags = flags;
  if (!bufsize) bufsize = iobuf_bufsize;
  if (!buffer) {
    if ((buffer = malloc(bufsize)) == 0) return 0;
    io->flags |= IOBUF_NEEDSFREE;
  }
  return 1;
}
