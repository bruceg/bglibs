#include "iobuf.h"

int obuf_init(obuf* out, int fd, int do_close, unsigned bufsize)
{
  out->bufpos = 0;
  out->count = 0;
  return iobuf_init(&(out->io), fd, do_close, bufsize, 0);
}
