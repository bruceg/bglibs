#include <errno.h>
#include <unistd.h>
#include "iobuf.h"

static const char errmsg[] = "ibuf_refill called with non-empty buffer!\n";

int ibuf_refill(ibuf* in)
{
  iobuf* io;
  unsigned oldlen;
  unsigned rd;

  io = &(in->io);
  if (io->flags) return 0;
  if (io->bufstart != 0) {
    if (io->bufstart < io->buflen) {
      write(1, errmsg, sizeof errmsg);
      _exit(1);
      /* io->buflen -= io->bufstart; */
      /* memcpy(io->buffer, io->buffer+io->bufstart, io->buflen); */
    }
    else
      io->buflen = 0;
    io->bufstart = 0;
  }
  oldlen = io->buflen;
  if(io->buflen < io->bufsize) {
    if (io->timeout && !iobuf_timeout(io, 0)) return 0;
    rd = in->readfn(io->fd, io->buffer+io->buflen, io->bufsize-io->buflen);
    if(rd == (unsigned)-1)
      IOBUF_SET_ERROR(io);
    else if(rd == 0)
      io->flags |= IOBUF_EOF;
    else {
      io->buflen += rd;
      io->offset += rd;
    }
  }
  return io->buflen > oldlen;
}
