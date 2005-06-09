#include <unistd.h>

#include "msg.h"
#include "wrap.h"

void wrap_chdir(const char* dir)
{
  if (chdir(dir) != 0)
    die3sys(wrap_exit, "Could not chdir to '", dir, "'");
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
const char program[] = "testprog";
const int msg_show_pid = 0;
void selftest(void)
{
  wrap_exit = 0;
  wrap_chdir(".");
  obuf_putsflush(&outbuf, "worked\n");
  wrap_chdir("/this/dir/should/not/exist");
}
#endif
#ifdef SELFTEST_EXP
worked
testprog: Fatal: Could not chdir to '/this/dir/should/not/exist': No such file or directory
#endif
