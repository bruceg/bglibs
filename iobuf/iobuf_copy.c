#include "iobuf.h"

/** Copy all the data from an \c ibuf to an \c obuf. */
int iobuf_copy(ibuf* in, obuf* out)
{
  char buf[iobuf_bufsize];
  if (ibuf_eof(in)) return 1;
  if (ibuf_error(in) || obuf_error(out)) return 0;
  do {
    if (!ibuf_read_large(in, buf, sizeof buf) && in->count == 0) break;
    if (!obuf_write_large(out, buf, in->count)) return 0;
  } while (!ibuf_eof(in));
  return ibuf_eof(in);
}

/** Copy all the data from an \c ibuf to an \c obuf, and flush the
    \c obuf after writing is completed. */
int iobuf_copyflush(ibuf* in, obuf* out)
{
  if (!iobuf_copy(in, out)) return 0;
  return obuf_flush(out);
}
