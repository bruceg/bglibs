#include <string.h>
#include "iobuf.h"

int obuf_putsflush(obuf* out, const char* str)
{
  return obuf_puts(out, str) && obuf_flush(out);
}
