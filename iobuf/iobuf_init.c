#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include "iobuf.h"

unsigned iobuf_bufsize = 8192;

int iobuf_init(iobuf* io, int fd, unsigned bufsize, char* buffer, unsigned flags)
{
  memset(io, 0, sizeof *io);
  if (!bufsize) bufsize = iobuf_bufsize;
  if (!buffer) {
    if ((buffer = mmap(0, bufsize, PROT_READ|PROT_WRITE,
		       MAP_PRIVATE|MAP_ANONYMOUS, 0, 0)) != 0)
      flags |= IOBUF_NEEDSMUNMAP;
    else {
      if ((buffer = malloc(bufsize)) == 0) return 0;
      flags |= IOBUF_NEEDSFREE;
    }
  }
  io->fd = fd;
  io->buffer = buffer;
  io->bufsize = bufsize;
  io->flags = flags;
  return 1;
}
