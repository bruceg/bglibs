#include "iobuf.h"

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

int iobuf_copyflush(ibuf* in, obuf* out)
{
  if (!iobuf_copy(in, out)) return 0;
  return obuf_flush(out);
}
