#include <unistd.h>
#include "iobuf.h"

int ibuf_init(ibuf* in, int fd, ibuf_fn fn, int do_close, unsigned bufsize)
{
  if (fn == 0) fn = (ibuf_fn)read;
  in->count = 0;
  in->readfn = fn;
  return iobuf_init(&(in->io), fd, do_close, bufsize, 0);
}
