#include <errno.h>
#include <string.h>
#include "iobuf/iobuf.h"
#include "err.h"

static const char colon[] = ": ";

void err_message(const char* type,
		 const char* a, const char* b, const char* c, const char* d,
		 int showsys)
{
  obuf_puts(&errbuf, program);
  obuf_puts(&errbuf, colon);
  obuf_puts(&errbuf, type);
  obuf_puts(&errbuf, colon);
  if (a) obuf_puts(&errbuf, a);
  if (b) obuf_puts(&errbuf, b);
  if (c) obuf_puts(&errbuf, c);
  if (d) obuf_puts(&errbuf, d);
  if (showsys) {
    obuf_puts(&errbuf, colon);
    obuf_puts(&errbuf, strerror(errno));
  }
  obuf_putsflush(&errbuf, "\n");
}
