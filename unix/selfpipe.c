#include <sys/types.h>
#include <unistd.h>

#include "sysdeps.h"
#include "unix/nonblock.h"
#include "unix/sig.h"

static int fds[2];

static void catch_child(int signal)
{
  write(fds[1], fds, 1);
  signal = 0;
}

int selfpipe_init(void)
{
  if (pipe(fds) == -1) return -1;
  if (!nonblock_on(fds[0]) || !nonblock_on(fds[1])) return -1;
  sig_child_catch(catch_child);
  return fds[0];
}

void selfpipe_close(void)
{
  close(fds[0]);
  close(fds[1]);
  sig_child_default();
}

#ifdef SELFTEST_MAIN
#include <errno.h>
#include <signal.h>
#include "selftest.c"
void puti(const char* word, long i)
{
  obuf_put2s(&outbuf, word, ": ");
  obuf_puti(&outbuf, i);
  obuf_endl(&outbuf);
}
MAIN
{
  int buf;
  int fd = selfpipe_init();
  puti("FD[0]", fds[0]);
  puti("FD[1]", fds[1]);
  puti("Returned FD", fd);
  puti("read before SIGCHLD", read(fd, &buf, sizeof buf));
  puti("errno is EAGAIN", errno == EAGAIN);
  kill(0, SIGCHLD);
  puti("read after SIGCHLD", read(fd, &buf, sizeof buf));
  puti("second read", read(fd, &buf, sizeof buf));
  kill(0, SIGCHLD);
  kill(0, SIGCHLD);
  puti("read after two SIGCHLDs", read(fd, &buf, sizeof buf));
  puti("second read", read(fd, &buf, sizeof buf));
  selfpipe_close();
  puti("read after close", read(fd, &buf, sizeof buf));
  puti("errno is EBADF", errno == EBADF);
}
#endif
#ifdef SELFTEST_EXP
FD[0]: 3
FD[1]: 4
Returned FD: 3
read before SIGCHLD: -1
errno is EAGAIN: 1
read after SIGCHLD: 1
second read: -1
read after two SIGCHLDs: 2
second read: -1
read after close: -1
errno is EBADF: 1
#endif
