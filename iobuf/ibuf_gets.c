#include "iobuf.h"

int ibuf_gets(ibuf* in, char* data, unsigned datalen, char boundary)
{
  iobuf* io;
  int ch;
  
  io = &in->io;
  in->count = 0;
  while (datalen) {
    if (io->bufstart >= io->buflen) ibuf_refill(in);
    if (ibuf_timedout(in)) return 0;
    if (ibuf_eof(in) || ibuf_error(in)) break;
    in->count++;
    ch = io->buffer[io->bufstart++];
    *data++ = ch;
    if (ch == boundary) return 1;
  }
  return ibuf_eof(in);
}
