#include <signal.h>
#include "sig.h"
#include "sysdeps.h"

#define SIGMAX _NSIG

void sig_all_catch(signalfn fn)
{
  int i;
  for (i = 1; i < SIGMAX; i++)
    if (i != SIGPROF)
      sig_catch(i, fn);
}

void sig_all_default(void)
{
  int i;
  for (i = 1; i < SIGMAX; i++)
    if (i != SIGPROF)
      sig_default(i);
}

void sig_all_block(void)
{
  int i;
#ifdef HASSIGPROCMASK
  sigset_t set;
  sigemptyset(&set);
  for (i = 1; i < SIGMAX; i++)
    if (i != SIGPROF)
      sigaddset(&set, i);
  sigprocmask(SIG_BLOCK, &set, 0);
#else
  sigblock(~(1 << (SIGPROF-1)));
#endif
}

void sig_all_unblock(void)
{
#ifdef HASSIGPROCMASK
  sigset_t set;
  sigemptyset(&set);
  sigprocmask(SIG_UNBLOCK, &set, 0);
#else
  sigsetmask(0);
#endif
}
