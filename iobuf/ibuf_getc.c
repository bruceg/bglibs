#include "iobuf.h"

int ibuf_getc(ibuf* in, char* ch)
{
  iobuf* io;
  io = &(in->io);
  in->count = 0;
  if (io->bufstart >= io->buflen) ibuf_refill(in);
  if (ibuf_eof(in) || ibuf_error(in) || ibuf_timedout(in)) return 0;
  *ch = io->buffer[io->bufstart++];
  in->count = 1;
  return 1;
}
