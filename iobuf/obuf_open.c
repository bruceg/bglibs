#include "obuf.h"

/** Initialize an \c obuf by opening a file for writing. */
int obuf_open(obuf* out, const char* filename, int oflags, int mode,
	      unsigned bufsize)
{
  int fd;
  if ((fd = open(filename, O_WRONLY | oflags, mode)) == -1) return 0;
  return obuf_init(out, fd, 0, IOBUF_NEEDSCLOSE|IOBUF_SEEKABLE, bufsize);
}
