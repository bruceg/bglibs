#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include "iobuf.h"

int obuf_seek(obuf* out, unsigned offset)
{
  iobuf* io;
  unsigned buf_start;
  unsigned buf_end;
  
  io = &out->io;
  if (io->flags) return 0;
  
  buf_start = io->offset;
  buf_end = io->offset + io->buflen;
  if (offset >= buf_start && offset < buf_end)
    out->bufpos = offset - buf_start;
  else {
    if (!obuf_flush(out)) return 0;
    if (lseek(io->fd, offset, SEEK_SET) == -1) {
      io->errnum = errno;
      io->flags |= IOBUF_ERROR;
      return 0;
    }
    io->offset = offset;
  }
  out->count = 0;
  return 1;
}
