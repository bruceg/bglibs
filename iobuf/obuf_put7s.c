#include <string.h>
#include "iobuf.h"

int obuf_put7s(obuf* out, const char* s1, const char* s2, const char* s3,
	       const char* s4, const char* s5, const char* s6, const char* s7)
{
  if (s1 && !obuf_puts(out, s1)) return 0;
  if (s2 && !obuf_puts(out, s2)) return 0;
  if (s3 && !obuf_puts(out, s3)) return 0;
  if (s4 && !obuf_puts(out, s4)) return 0;
  if (s5 && !obuf_puts(out, s5)) return 0;
  if (s6 && !obuf_puts(out, s6)) return 0;
  if (s7 && !obuf_puts(out, s7)) return 0;
  return 1;
}
