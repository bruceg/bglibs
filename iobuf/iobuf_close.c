#include <sys/types.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include "iobuf.h"

/** Close an \c iobuf and deallocate the buffer. */
int iobuf_close(iobuf* io)
{
  int status;
  if (io->flags & IOBUF_NEEDSMUNMAP) {
    munmap(io->buffer, io->bufsize);
    io->buffer = 0;
  }
  else if (io->flags & IOBUF_NEEDSFREE) {
    free(io->buffer);
    io->buffer = 0;
  }
  status = 1;
  if ((io->flags & IOBUF_NEEDSCLOSE) && io->fd != -1)
    status = close(io->fd) != -1;
  io->fd = -1;
  return status;
}
