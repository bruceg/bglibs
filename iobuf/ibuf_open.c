#include <unistd.h>
#include "iobuf.h"

int ibuf_open(ibuf* in, const char* filename, unsigned bufsize)
{
  int fd;
  if ((fd = open(filename, O_RDONLY)) == -1) return 0;
  if (!ibuf_init(in, fd, 1, bufsize)) {
    close(fd);
    return 0;
  }
  return 1;
}
