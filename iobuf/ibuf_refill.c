#include <errno.h>
#include <unistd.h>
#include "iobuf.h"

int ibuf_refill(ibuf* in)
{
  iobuf* io;
  unsigned oldlen;
  unsigned rd;

  io = &(in->io);
  if (io->flags) return 0;
  if (io->bufstart != 0) {
    if (io->bufstart < io->buflen) {
      io->buflen -= io->bufstart;
      memcpy(io->buffer, io->buffer+io->bufstart, io->buflen);
    }
    else
      io->buflen = 0;
    io->bufstart = 0;
  }
  oldlen = io->buflen;
  if(io->buflen < io->bufsize) {
    rd = read(io->fd, io->buffer+io->buflen, io->bufsize-io->buflen);
    if(rd == -1) {
      io->errnum = errno;
      io->flags |= IOBUF_ERROR;
    }
    else if(rd == 0)
      io->flags |= IOBUF_EOF;
    else {
      io->buflen += rd;
      io->offset += rd;
    }
  }
  return io->buflen > oldlen;
}
