#include "iobuf.h"

int obuf_putc(obuf* out, char ch)
{
  iobuf* io;
  
  io = &out->io;
  if (io->flags) return 0;
  
  out->count = 0;
  io->buffer[out->bufpos++] = ch;
  if (out->bufpos >= io->buflen) io->buflen = out->bufpos;
  if (io->buflen >= io->bufsize && !obuf_flush(out)) return 0;
  out->count = 1;
  return 1;
}
