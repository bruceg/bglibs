#include "iobuf.h"

int ibuf_gets(ibuf* in, char* data, unsigned datalen, char boundary)
{
  iobuf* io;
  int ch;
  
  io = &in->io;
  in->count = 0;
  if (ibuf_eof(in) || ibuf_error(in) || ibuf_timedout(in)) return 0;
  while (datalen > 1) {
    if (io->bufstart >= io->buflen && !ibuf_refill(in)) {
      if (ibuf_eof(in)) break;
      return 0;
    }
    in->count++;
    ch = io->buffer[io->bufstart++];
    *data++ = ch;
    if (ch == boundary) break;
  }
  *data = 0;
  return 1;
}
