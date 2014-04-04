#include <unistd.h>

#include "msg.h"
#include "wrap.h"

void wrap_chdir(const char* dir)
{
  if (chdir(dir) != 0)
    die3sys(wrap_exit, "Could not chdir to '", dir, "'");
}

#ifdef SELFTEST_MAIN
MAIN
{
  wrap_exit = 0;
  wrap_chdir(".");
  obuf_putsflush(&outbuf, "worked\n");
  wrap_chdir("/this/dir/should/not/exist");
}
#endif
#ifdef SELFTEST_EXP
worked
selftest: Fatal: Could not chdir to '/this/dir/should/not/exist': No such file or directory
#endif
