#include "iobuf.h"

/** Retrieve the next character in the \c ibuf without advancing the
    current read position. */
int ibuf_peek(ibuf* in, char* ch)
{
  iobuf* io;
  io = &(in->io);
  if (ibuf_eof(in) || ibuf_error(in)) return 0;
  if (io->bufstart >= io->buflen && !ibuf_refill(in)) return 0;
  *ch = io->buffer[io->bufstart];
  return 1;
}
