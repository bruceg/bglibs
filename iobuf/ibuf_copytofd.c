/* $Id$ */
#include <unistd.h>
#include "ibuf.h"

/** Copy all the data from an \c ibuf to a file descriptor. */
int ibuf_copytofd(ibuf* in, int out)
{
  const char* buf;
  long len;
  long wr;
  if (ibuf_eof(in)) return 1;
  if (ibuf_error(in)) return 0;
  in->count = 0;
  for (;;) {
    buf = in->io.buffer + in->io.bufstart;
    len = in->io.buflen - in->io.bufstart;
    while (len > 0) {
      if ((wr = write(out, buf, len)) <= 0)
	return 0;
      buf += wr;
      len -= wr;
      in->count += wr;
    }
    in->io.bufstart = in->io.buflen;
    if (!ibuf_refill(in))
      return ibuf_eof(in);
  }
}
