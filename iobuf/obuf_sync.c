#include <errno.h>
#include <unistd.h>
#include "iobuf.h"

int obuf_sync(obuf* out)
{
  iobuf* io;
  if (!obuf_flush(out)) return 0;
  io = &out->io;
  if (fsync(io->fd) == -1) {
    io->flags |= IOBUF_ERROR;
    io->errnum = errno;
    return 0;
  }
  return 1;
}
