#include <errno.h>
#include <unistd.h>
#include "iobuf.h"

int obuf_write_large(obuf* out, const char* data, unsigned datalen)
{
  iobuf* io;
  unsigned wr;
  
  io = &out->io;
  if (io->flags) return 0;
  out->count = 0;
  if (!obuf_flush(out)) return 0;
  
  while (datalen > 0) {
    if (io->timeout && !iobuf_timeout(io, 1)) return 0;
    if ((wr = write(io->fd, data, datalen)) == -1) IOBUF_SET_ERROR(io);
    datalen -= wr;
    data += wr;
    io->offset += wr;
    out->count += wr;
  }
  return 1;
}

int obuf_write(obuf* out, const char* data, unsigned datalen)
{
  iobuf* io;
  unsigned avail;
  
  io = &out->io;
  if (io->flags) return 0;
  if (datalen >= io->bufsize) return obuf_write_large(out, data, datalen);

  out->count = 0;
  avail = io->bufsize - out->bufpos;
  while (datalen >= avail) {
    memcpy(io->buffer+out->bufpos, data, avail);
    out->bufpos = io->bufsize;
    io->buflen = io->bufsize;
    datalen -= avail;
    data += avail;
    if (!obuf_flush(out)) return 0;
    out->count += avail;
    avail = io->bufsize - out->bufpos;
  }
  
  memcpy(io->buffer+out->bufpos, data, datalen);
  out->count += datalen;
  out->bufpos += datalen;
  if (out->bufpos > io->buflen) io->buflen = out->bufpos;
  return 1;
}
