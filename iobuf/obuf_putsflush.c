#include <string.h>
#include "iobuf.h"

/** Write a C string to the \c obuf and flush it. */
int obuf_putsflush(obuf* out, const char* str)
{
  return obuf_puts(out, str) && obuf_flush(out);
}
