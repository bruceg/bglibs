#include <sys/types.h>
#include <unistd.h>

#include "sysdeps.h"
#include "unix/nonblock.h"
#include "unix/sig.h"

static int fds[2] = { -1, -1 };

/** Catch a signal and write it to the self-pipe.
 *
 * This routine can be used with sig_*_catch() to send other signals
 * through the self-pipe.
 */
void selfpipe_catch_signal(int signal)
{
  char c = signal;
  write(fds[1], &c, 1);
}

/** Set up a self-pipe for catching child exit events.
 *
 * This function opens up a pipe within the program used to safely
 * handle exiting child processes.  Every time a child exits, a single
 * byte is written to the pipe.  The resulting file descriptor is
 * statically assigned, so do not call this function more than once
 * within a single program.  The file descriptor is set to non-blocking
 * mode to prevent blocking when reading from it.
 *
 * To use the file descriptor, use either \c poll or \c select to
 * determine when it is readable.  When it becomes readable, read the
 * data written to it (and throw it away), and use \c wait or equivalent
 * to catch the \c exit value from the child process(es).
 *
 * \return -1 if an error occurred, otherwise the return value is the
 * file descriptor opened for reading.
 */
int selfpipe_init(void)
{
  if (pipe(fds) == -1) return -1;
  if (!nonblock_on(fds[0]) || !nonblock_on(fds[1])) {
    close(fds[0]);
    close(fds[1]);
    return -1;
  }
  sig_child_catch(selfpipe_catch_signal);
  return fds[0];
}

/** Shut down the self-pipe. */
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
  puti("FD[0] >= 0", fds[0] >= 0);
  puti("FD[1] >= 0", fds[1] >= 0);
  puti("FD[0] != FD[1]", fds[0] != fds[1]);
  puti("Returned FD == FD[0]", fd == fds[0]);
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
