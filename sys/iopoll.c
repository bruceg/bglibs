#include <errno.h>
#include "sysdeps.h"

#ifdef IOPOLL_SELECT

int iopoll(iopoll_fd* fds, unsigned nfds, unsigned long timeout)
{
  struct timeval tv;
  fd_set rfds;
  fd_set wfds;
  int fdmax;
  iopoll_fd* io;
  int fd;
  unsigned i;
  int r;
  
  FD_ZERO(&rfds);
  FD_ZERO(&wfds);

  for (fdmax = 0, i = 0, io = fds; i < nfds; ++i, ++io) {
    fd = io->fd;
    if (fd < 0) continue;
    if (fd >= fdmax) fdmax = fd + 1;
    if (io->events & IOPOLL_READ) FD_SET(fd, &rfds);
    if (io->events & IOPOLL_WRITE) FD_SET(fd, &wfds);
  }

  tv.tv_sec = timeout / 1000;
  tv.tv_usec = (timeout % 1000) * 1000;

  if ((r = select(fdmax, &rfds, &wfds, (fd_set*)0, &tv)) <= 0) return r;

  for (i = 0, io = fds; i < nfds; ++i, ++io) {
    fd = io->fd;
    if (fd < 0) continue;
    if ((io->events & IOPOLL_READ) && FD_ISSET(fd, &rfds))
      io->revents |= IOPOLL_READ;
    if ((io->events & IOPOLL_WRITE) && FD_ISSET(fd, &wfds))
      io->revents |= IOPOLL_WRITE;
  }
  return r;
}

#endif

int iopoll_restart(iopoll_fd* fds, unsigned nfds, unsigned long timeout)
{
  int f;
  while ((f = iopoll(fds, nfds, timeout)) == -1)
    if (errno != EAGAIN && errno != EINTR)
      break;
  return f;
}
