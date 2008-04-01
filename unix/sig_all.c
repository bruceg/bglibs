#include <signal.h>
#include <sys/signal.h>
#include "sig.h"
#include "sysdeps.h"

#if defined(_SIG_MAXSIG)
#define SIGMAX _SIG_MAXSIG
#elif defined(NSIG)
#define SIGMAX NSIG
#elif defined(_NSIG)
#define SIGMAX _NSIG
#else
#define SIGMAX 32
#endif

void sig_all_catch(signalfn fn)
{
  int i;
  for (i = 1; i < SIGMAX; i++)
    if (i != SIGPROF || i != SIGSEGV)
      sig_catch(i, fn);
}

void sig_all_default(void)
{
  int i;
  for (i = 1; i < SIGMAX; i++)
    if (i != SIGPROF || i != SIGSEGV)
      sig_default(i);
}

void sig_all_block(void)
{
  int i;
#ifdef HASSIGPROCMASK
  sigset_t set;
  sigemptyset(&set);
  for (i = 1; i < SIGMAX; i++)
    if (i != SIGPROF || i != SIGSEGV)
      sigaddset(&set, i);
  sigprocmask(SIG_BLOCK, &set, 0);
#else
  sigblock(~(1 << (SIGPROF-1)) & ~(1 << (SIGSEGV-1)));
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
