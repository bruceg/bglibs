#include <unistd.h>
#include "iobuf.h"

int ibuf_init(ibuf* in, int fd, ibuf_fn fn, unsigned flags, unsigned bufsize)
{
  if (fn == 0) fn = (ibuf_fn)read;
  in->count = 0;
  in->readfn = fn;
  return iobuf_init(&(in->io), fd, bufsize, 0, flags);
}
