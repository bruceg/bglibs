/* $Id$ */
#include <sys/types.h>
#include <unistd.h>

#include "sysdeps.h"
#include "unix/cloexec.h"
#include "unix/nonblock.h"
#include "unix/sig.h"
#include "unix/signalfd.h"

static int fds[2] = { -1, -1 };

static void signalfd_handler(int sig)
{
  write(fds[1], &sig, sizeof sig);
}

/** Initialize the signal file descriptor.
 *
 * This function creates a pipe through which signals will get passed.
 * This is a way of handling signals that avoids most problems posed by
 * UNIX signals.  When a signal is caught by this interface, the
 * numerical value of the signal is passed down the pipe as a native
 * integer.  The file descriptor is set to non-blocking mode to prevent
 * blocking when reading from it.
 *
 * To use the file descriptor, use either \c poll or \c select to
 * determine when it is readable.  When it becomes readable, read an
 * integer-sized block from it.  The value of this data indicates what
 * signal was caught.
 *
 * Due to the global nature of UNIX signal handling, only a single
 * signalfd interface may be active at any time.  Do not call \c
 * signalfd_init multiple times without calling \c signalfd_close first.
 *
 * \return -1 if an error occurred, otherwise the return value is the
 * file descriptor opened for reading.
 */
int signalfd_init(void)
{
  if (pipe(fds) == -1)
    return -1;
  if (!cloexec_on(fds[0])
      || !nonblock_on(fds[0])
      || !cloexec_on(fds[1])
      || !nonblock_on(fds[1]))
    signalfd_close();
  return fds[0];
}

/** Mark a signal as being caught through the signalfd interface. */
void signalfd_catch(int sig)
{
  sig_catch(sig, signalfd_handler);
}

/** Unmark a signal as being caught through the signalfd interface. */
void signalfd_uncatch(int sig)
{
  sig_default(sig);
}

/** Close the signalfd interface. */
void signalfd_close(void)
{
  close(fds[0]);
  close(fds[1]);
  fds[0] = fds[1] = -1;
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
  pid_t pid = getpid();
  puti("FD[0] >= 0", fds[0] >= 0);
  puti("FD[1] >= 0", fds[1] >= 0);
  puti("FD[0] != FD[1]", fds[0] != fds[1]);
  puti("Returned FD == FD[0]", fd == fds[0]);
  puti("read before SIGCHLD", read(fd, &buf, sizeof buf));
  puti("errno is EAGAIN", errno == EAGAIN);
  kill(pid, SIGCHLD);
  puti("read after SIGCHLD", read(fd, &buf, sizeof buf));
  puti("second read", read(fd, &buf, sizeof buf));
  kill(pid, SIGCHLD);
  kill(pid, SIGCHLD);
  puti("read after two SIGCHLDs", read(fd, &buf, sizeof buf));
  puti("second read", read(fd, &buf, sizeof buf));
  selfpipe_close();
  puti("read after close", read(fd, &buf, sizeof buf));
  puti("errno is EBADF", errno == EBADF);
}
#endif
#ifdef SELFTEST_EXP
FD[0] >= 0: 1
FD[1] >= 0: 1
FD[0] != FD[1]: 1
Returned FD == FD[0]: 1
read before SIGCHLD: -1
errno is EAGAIN: 1
read after SIGCHLD: 1
second read: -1
read after two SIGCHLDs: 2
second read: -1
read after close: -1
errno is EBADF: 1
#endif
