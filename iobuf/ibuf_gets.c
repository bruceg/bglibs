#include "iobuf.h"

int ibuf_gets(ibuf* in, char* data, unsigned datalen, char boundary)
{
  iobuf* io;
  io = &in->io;
  in->count = 0;
  while (datalen) {
    if (io->bufstart >= io->buflen) ibuf_refill(in);
    if (ibuf_eof(in) || ibuf_error(in)) break;
    in->count++;
    if ((*data++ = io->buffer[io->bufstart++]) == boundary) return 1;
  }
  return ibuf_eof(in);
}
