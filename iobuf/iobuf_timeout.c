#include <errno.h>
#include "iobuf.h"
#include "iopoll.h"

int iobuf_timeout(iobuf* io, int poll_out)
{
  iopoll_fd pfd;
  int result;
  
  if (!io->timeout) return 1;

  pfd.fd = io->fd;
  pfd.events = poll_out ? IOPOLL_WRITE : IOPOLL_READ;
  while ((result = iopoll(&pfd, 1, io->timeout)) == -1) {
    if (errno == EAGAIN) continue;
    IOBUF_SET_ERROR(io);
  }
  if (result) return 1;
  io->flags |= IOBUF_TIMEOUT;
  return 0;
}
