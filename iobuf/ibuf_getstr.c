#include "iobuf.h"
#include "str/str.h"

int ibuf_getstr(ibuf* in, str* s, char boundary)
{
  iobuf* io;
  int ch;
  
  io = &in->io;
  in->count = 0;
  str_truncate(s, 0);
  if (ibuf_eof(in) || ibuf_error(in) || ibuf_timedout(in)) return 0;
  for (;;) {
    if (io->bufstart >= io->buflen && !ibuf_refill(in)) {
      if (ibuf_eof(in)) break;
      return 0;
    }
    in->count++;
    ch = io->buffer[io->bufstart++];
    if (!str_catc(s, ch)) return 0;
    if (ch == boundary) break;
  }
  return 1;
}
