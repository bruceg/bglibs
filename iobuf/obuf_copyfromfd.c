#include <unistd.h>
#include "obuf.h"

/** Copy all the data from an \c ibuf to an \c obuf. */
int obuf_copyfromfd(int in, obuf* out)
{
  long rd;
  if (obuf_error(out)) return 0;
  out->count = 0;
  for (;;) {
    if ((rd = read(in,
		   out->io.buffer + out->bufpos,
		   out->io.bufsize - out->bufpos)) == -1)
      return 0;
    if (rd == 0)
      break;
    out->bufpos += rd;
    if (out->io.buflen < out->bufpos)
      out->io.buflen = out->bufpos;
    if (!obuf_flush(out))
      return 0;
    out->count += rd;
  }
  return 1;
}
