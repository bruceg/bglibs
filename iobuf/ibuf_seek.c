#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include "iobuf.h"

/** Set the effective read position. */
int ibuf_seek(ibuf* in, unsigned offset)
{
  iobuf* io;
  unsigned buf_start;
  
  io = &(in->io);
  buf_start = io->offset - io->buflen;
  if (offset >= buf_start && offset <= io->offset)
    io->bufstart = offset - buf_start;
  else {
    if (lseek(io->fd, offset, SEEK_SET) != (off_t)offset)
      IOBUF_SET_ERROR(io);
    io->offset = offset;
    io->buflen = 0;
    io->bufstart = 0;
  }
  in->count = 0;
  io->flags &= ~IOBUF_EOF;
  return 1;
}
