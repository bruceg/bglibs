#include <unistd.h>
#include "ibuf.h"

/** Initialize an \c ibuf by opening a file for reading. */
int ibuf_open(ibuf* in, const char* filename, unsigned bufsize)
{
  int fd;
  if ((fd = open(filename, O_RDONLY)) == -1) return 0;
  if (!ibuf_init(in, fd, 0, IOBUF_SEEKABLE|IOBUF_NEEDSCLOSE, bufsize)) {
    close(fd);
    return 0;
  }
  return 1;
}
