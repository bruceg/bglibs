#include <string.h>
#include "iobuf.h"

int obuf_put4s(obuf* out, const char* s1, const char* s2, const char* s3,
	       const char* s4)
{
  if (s1 && !obuf_puts(out, s1)) return 0;
  if (s2 && !obuf_puts(out, s2)) return 0;
  if (s3 && !obuf_puts(out, s3)) return 0;
  if (s4 && !obuf_puts(out, s4)) return 0;
  return 1;
}
