#include <unistd.h>
#include "iobuf.h"

int obuf_init(obuf* out, int fd, obuf_fn fn, int do_close, unsigned bufsize)
{
  if (fn == 0) fn = (obuf_fn)write;
  out->bufpos = 0;
  out->count = 0;
  out->writefn = fn;
  return iobuf_init(&(out->io), fd, do_close, bufsize, 0);
}
