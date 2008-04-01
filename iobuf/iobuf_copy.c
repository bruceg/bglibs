#include "iobuf.h"

/** Copy all the data from an \c ibuf to an \c obuf. */
int iobuf_copy(ibuf* in, obuf* out)
{
  if (ibuf_eof(in)) return 1;
  if (ibuf_error(in) || obuf_error(out)) return 0;
  for (;;) {
    if (!obuf_write_large(out,
			  in->io.buffer+in->io.bufstart,
			  in->io.buflen-in->io.bufstart))
      return 0;
    in->io.bufstart = in->io.buflen;
    if (!ibuf_refill(in))
      return ibuf_eof(in);
  }
}

/** Copy all the data from an \c ibuf to an \c obuf, and flush the
    \c obuf after writing is completed. */
int iobuf_copyflush(ibuf* in, obuf* out)
{
  if (!iobuf_copy(in, out)) return 0;
  return obuf_flush(out);
}
