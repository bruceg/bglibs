#include <stdlib.h>
#include <unistd.h>
#include "iobuf.h"

int iobuf_close(iobuf* io)
{
  int status;
  if (io->do_free) {
    free(io->buffer);
    io->buffer = 0;
  }
  status = 1;
  if (io->do_close && io->fd != -1)
    status = close(io->fd) != -1;
  io->fd = -1;
  return status;
}
