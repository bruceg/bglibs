/* $Id$ */
#include "msg.h"
#include "wrap.h"

void wrap_alloc(const void* p)
{
  if (p == 0)
    die_oom(wrap_exit);
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
const char program[] = "testprog";
const int msg_show_pid = 0;
void selftest(void)
{
  wrap_exit = 0;
  wrap_alloc(selftest);
  obuf_putsflush(&outbuf, "worked\n");
  wrap_alloc(0);
}
#endif
#ifdef SELFTEST_EXP
worked
testprog: Fatal: Out of memory
#endif
