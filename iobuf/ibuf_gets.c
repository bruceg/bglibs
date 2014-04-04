#include "ibuf.h"

/** Read a line from the \c ibuf into a C string. */
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

#ifdef SELFTEST_MAIN
#include <unistd.h>

ibuf in = {
  { -1, "a\0bcd\0efgh", 10, 10, 0, 0, 0, 0, 0 },
  0, (ibuf_fn)read
};

MAIN
{
  char buf[999];
  while (ibuf_gets(&in, buf, sizeof buf, 0))
    obuf_putf(&outbuf, "u{:}s{\n}", in.count, buf);
}
#endif
#ifdef SELFTEST_EXP
2:a
4:bcd
#endif
