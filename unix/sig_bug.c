#include <signal.h>
#include "sig.h"

void sig_bug_catch(signalfn fn)
{
  sig_catch(SIGILL, fn);
  sig_catch(SIGABRT, fn);
  sig_catch(SIGFPE, fn);
  sig_catch(SIGSEGV, fn);
#ifdef SIGEMT
  sig_catch(SIGEMT, fn);
#endif
#ifdef SIGBUS
  sig_catch(SIGBUS, fn);
#endif
#ifdef SIGSYS
  sig_catch(SIGSYS, fn);
#endif
#ifdef SIGSTKFLT
  sig_catch(SIGSTKFLT, fn);
#endif
}
