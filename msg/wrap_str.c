#include "msg.h"
#include "wrap.h"

void wrap_str(int result)
{
  if (!result)
    die_oom(wrap_exit);
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
MAIN
{
  wrap_exit = 0;
  wrap_str(1);
  obuf_putsflush(&outbuf, "worked\n");
  wrap_str(0);
}
#endif
#ifdef SELFTEST_EXP
worked
selftest: Fatal: Out of memory
#endif
