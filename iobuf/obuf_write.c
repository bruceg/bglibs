/* $Id$ */
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "obuf.h"

/** Write a large block of data to the \c obuf, avoiding copying. */
int obuf_write_large(obuf* out, const char* data, unsigned datalen)
{
  iobuf* io;
  unsigned wr;
  
  io = &out->io;
  if (iobuf_bad(io)) return 0;
  out->count = 0;
  if (!obuf_flush(out)) return 0;
  
  while (datalen > 0) {
    if (io->timeout && !iobuf_timeout(io, 1)) return 0;
    if ((wr = out->writefn(io->fd, data, datalen)) == (unsigned)-1)
      IOBUF_SET_ERROR(io);
    datalen -= wr;
    data += wr;
    io->offset += wr;
    out->count += wr;
  }
  return 1;
}

/** Write a block of data to the \c obuf */
int obuf_write(obuf* out, const char* data, unsigned datalen)
{
  iobuf* io;
  unsigned avail;
  
  io = &out->io;
  if (iobuf_bad(io)) return 0;
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

#ifdef SELFTEST_MAIN
#include "selftest.c"
MAIN
{
  obuf buf;
  obuf_init(&buf, 1, 0, 0, 8);
  /* Test proper merging of short writes */
  obuf_puts(&buf, "0123"); write(1,"brk\n",4);
  obuf_puts(&buf, "456789\n"); write(1,"brk\n",4);
  /* Test pass-through of large writes */
  obuf_write(&buf, "01234567\n", 9); write(1,"brk\n",4);
}
#endif
#ifdef SELFTEST_EXP
brk
01234567brk
89
01234567
brk
#endif
