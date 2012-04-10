#ifndef SELFTEST_C

#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "iobuf.h"
#include "msg.h"
#include "path/path.h"
#include "str.h"

const char program[] = "selftest";
const int msg_show_pid = 0;

void NL(void) { obuf_putc(&outbuf, LF); obuf_flush(&outbuf); }

void debugstr(const str* s)
{
  obuf_puts(&outbuf, "len=");
  obuf_putu(&outbuf, s->len);
  obuf_puts(&outbuf, " size=");
  obuf_putu(&outbuf, s->size);
  if (s->s == 0)
    obuf_puts(&outbuf, " s is NULL");
  else {
    obuf_puts(&outbuf, " s=");
    obuf_putstr(&outbuf, s);
  }
  NL();
}

int debugstrfn(int result, const str* s)
{
  obuf_puts(&outbuf, "result=");
  obuf_puti(&outbuf, result);
  obuf_putc(&outbuf, ' ');
  debugstr(s);
  return result;
}

int debugfn(int result)
{
  obuf_puts(&outbuf, "result=");
  obuf_puti(&outbuf, result);
  NL();
  return result;
}

int debugsys(int result)
{
  obuf_puts(&outbuf, "result=");
  obuf_puti(&outbuf, result);
  NL();
  if (result == -1) {
    obuf_puts(&outbuf, strerror(errno));
    NL();
  }
  return result;
}

void makefile(str* fn, const char* content)
{
  long len = strlen(content);
  int fd = path_mktemp("./selftest.tmpfile", fn);
  if (fd < 0) die1(1, "Could not create temporary file");
  if (write(fd, content, len) != len || close(fd) != 0) {
    unlink(fn->s);
    die1(1, "Could not write temporary file");
  }
}

#define MAIN void selftest(void)
extern MAIN;

int main(void)
{
  selftest();
  obuf_flush(&outbuf);
  obuf_flush(&errbuf);
  return 0;
}

#endif
