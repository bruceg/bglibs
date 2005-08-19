/* $Id$ */
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "iobuf/obuf.h"
#include "msg.h"

static pid_t pid = 0;

void msg_commonfv(const char* type,
		  int showsys,
		  const char* format,
		  va_list ap)
{
  obuf_puts(&errbuf, program);
  if (msg_show_pid) {
    if (!pid || msg_show_pid < 0) pid = getpid();
    obuf_putc(&errbuf, '[');
    obuf_putu(&errbuf, pid);
    obuf_putc(&errbuf, ']');
  }
  obuf_puts(&errbuf, ": ");
  if (type) {
    obuf_puts(&errbuf, type);
    obuf_puts(&errbuf, ": ");
  }

  obuf_putfv(&errbuf, format, ap);

  if (showsys) {
    obuf_puts(&errbuf, ": ");
    obuf_puts(&errbuf, strerror(errno));
  }
  obuf_endl(&errbuf);
}

void msg_commonf(const char* type,
		 int showsys,
		 const char* format,
		 ...)
{
  va_list ap;
  va_start(ap, format);
  msg_commonfv(type, showsys, format, ap);
  va_end(ap);
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
const char program[] = "testprog";
const int msg_show_pid = 0;
void selftest(void)
{
  warnf("ss", "a", "b");
}
#endif
#ifdef SELFTEST_EXP
testprog: Warning: ab
#endif
