#include <errno.h>
#include <sys/poll.h>
#include "iobuf.h"

int iobuf_timeout(iobuf* io, int poll_out)
{
  struct pollfd pfd;
  int result;
  
  if (!io->timeout) return 1;

  pfd.fd = io->fd;
  pfd.events = poll_out ? POLLOUT : POLLIN;
  while ((result = poll(&pfd, 1, io->timeout)) == -1) {
    if (errno == EAGAIN) continue;
    IOBUF_SET_ERROR(io);
  }
  if (result) return 1;
  io->flags |= IOBUF_TIMEOUT;
  return 0;
}
