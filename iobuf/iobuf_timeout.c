#include <errno.h>
#include <sys/poll.h>
#include "iobuf.h"

int iobuf_timeout(iobuf* io, int poll_out)
{
  struct pollfd pfd;

  if (!io->timeout) return 1;

  pfd.fd = io->fd;
  pfd.events = poll_out ? POLLOUT : POLLIN;
  for (;;) {
    switch (poll(&pfd, 1, io->timeout)) {
    case -1:
      if (errno == EAGAIN) continue;
      IOBUF_SET_ERROR(io);
    case 0:
      io->flags |= IOBUF_TIMEOUT;
      return 0;
    }
  }
  return 1;
}
