#include "iobuf.h"

int ibuf_peek(ibuf* in, char* ch)
{
  iobuf* io;
  io = &(in->io);
  if (io->bufstart >= io->buflen) ibuf_refill(in);
  if (ibuf_eof(in) || ibuf_error(in) || ibuf_timeout(in)) return 0;
  *ch = io->buffer[io->bufstart];
  return 1;
}
