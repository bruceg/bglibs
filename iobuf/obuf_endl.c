/* $Id$ */
#include "obuf.h"

/** Write a newline to the \c obuf and flush it. */
int obuf_endl(obuf* out)
{
  return obuf_putc(out, LF) && obuf_flush(out);
}
