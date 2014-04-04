#include "msg.h"
#include "wrap.h"

void wrap_alloc(const void* p)
{
  if (p == 0)
    die_oom(wrap_exit);
}

#ifdef SELFTEST_MAIN
MAIN
{
  wrap_exit = 0;
  wrap_alloc(selftest);
  obuf_putsflush(&outbuf, "worked\n");
  wrap_alloc(0);
}
#endif
#ifdef SELFTEST_EXP
worked
selftest: Fatal: Out of memory
#endif
