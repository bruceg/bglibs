#include "iobuf.h"
#include "str.h"

/** Read the remainder of the \c ibuf into the \c str. */
int ibuf_readall(ibuf* in, struct str* out)
{
  if (ibuf_eof(in)) return 1;
  if (ibuf_error(in)) return 0;
  for (;;) {
    if (!str_catb(out,
		  in->io.buffer+in->io.bufstart,
		  in->io.buflen-in->io.bufstart))
      return 0;
    in->io.bufstart = in->io.buflen;
    if (!ibuf_refill(in))
      return ibuf_eof(in);
  }
}
