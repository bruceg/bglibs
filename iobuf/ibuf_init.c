#include "iobuf.h"

int ibuf_init(ibuf* in, int fd, int do_close, unsigned bufsize)
{
  in->count = 0;
  return iobuf_init(&(in->io), fd, do_close, bufsize, 0);
}
