#include "iobuf.h"

int obuf_open(obuf* out, const char* filename, int flags, int mode,
	      unsigned bufsize)
{
  int fd;
  if ((fd = open(filename, O_WRONLY | flags, mode)) == -1) return 0;
  return obuf_init(out, fd, 1, bufsize);
}
