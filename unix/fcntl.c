/* unix/fcntl.c - fcntl wrapper self-tests. */

#ifdef SELFTEST_MAIN
#include <unistd.h>
#include <fcntl.h>
#include "unix.h"

static void fdtest(int fd, int flag)
{
  int flags;
  flags = fcntl(fd, F_GETFD, 0);
  obuf_putsflush(&outbuf, (flags & flag) ? "set\n" : "cleared\n");
}

static void fltest(int fd, int flag)
{
  int flags;
  flags = fcntl(fd, F_GETFL, 0);
  obuf_putsflush(&outbuf, (flags & flag) ? "set\n" : "cleared\n");
}

MAIN
{
  fcntl_fd_off(0, FD_CLOEXEC); fdtest(0, FD_CLOEXEC);
  fcntl_fd_on(0, FD_CLOEXEC); fdtest(0, FD_CLOEXEC);
  fcntl_fd_off(0, FD_CLOEXEC); fdtest(0, FD_CLOEXEC);
  fcntl_fd_on(0, FD_CLOEXEC); fdtest(0, FD_CLOEXEC);

  fcntl_fl_off(0, O_NONBLOCK); fltest(0, O_NONBLOCK);
  fcntl_fl_on(0, O_NONBLOCK); fltest(0, O_NONBLOCK);
  fcntl_fl_off(0, O_NONBLOCK); fltest(0, O_NONBLOCK);
  fcntl_fl_on(0, O_NONBLOCK); fltest(0, O_NONBLOCK);
}
#endif
#ifdef SELFTEST_EXP
cleared
set
cleared
set
cleared
set
cleared
set
#endif
