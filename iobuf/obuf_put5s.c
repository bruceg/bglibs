#include <string.h>
#include "iobuf.h"

int obuf_put5s(obuf* out, const char* s1, const char* s2, const char* s3,
	       const char* s4, const char* s5)
{
  return
    obuf_puts(out, s1) &&
    obuf_puts(out, s2) &&
    obuf_puts(out, s3) &&
    obuf_puts(out, s4) &&
    obuf_puts(out, s5);
}
