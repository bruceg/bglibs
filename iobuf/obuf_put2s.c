#include <string.h>
#include "iobuf.h"

int obuf_put2s(obuf* out, const char* s1, const char* s2)
{
  return obuf_puts(out, s1) && obuf_puts(out, s2);
}
