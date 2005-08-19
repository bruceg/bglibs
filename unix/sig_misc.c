/* $Id$ */
#include <signal.h>
#include "sig.h"

void sig_misc_ignore(void)
{
  sig_ignore(SIGHUP);
  sig_ignore(SIGINT);
  sig_ignore(SIGQUIT);
  sig_ignore(SIGUSR1);
  sig_ignore(SIGUSR2);
#ifdef SIGXCPU
  sig_ignore(SIGXCPU);
#endif
#ifdef SIGXFSZ
  sig_ignore(SIGXFSZ);
#endif
  sig_ignore(SIGVTALRM);
  sig_ignore(SIGPROF);
}
