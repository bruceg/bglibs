#include <string.h>

#include "iobuf/iobuf.h"
#include "msg/msg.h"
#include "str/str.h"

void NL(void) { obuf_putc(&outbuf, LF); }

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

#define MAIN void selftest(void)
MAIN;

int main(void)
{
  selftest();
  obuf_flush(&outbuf);
  obuf_flush(&errbuf);
  return 0;
}
