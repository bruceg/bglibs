#include <stdlib.h>
#include <unistd.h>
#include "iobuf.h"

int iobuf_close(iobuf* io)
{
  int status;
  if (io->flags & IOBUF_NEEDSFREE) {
    free(io->buffer);
    io->buffer = 0;
  }
  status = 1;
  if ((io->flags & IOBUF_NEEDSCLOSE) && io->fd != -1)
    status = close(io->fd) != -1;
  io->fd = -1;
  return status;
}
