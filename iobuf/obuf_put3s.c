#include <string.h>
#include "iobuf.h"

/** Write 3 C strings to the \c obuf */
int obuf_put3s(obuf* out, const char* s1, const char* s2, const char* s3)
{
  if (s1 && !obuf_puts(out, s1)) return 0;
  if (s2 && !obuf_puts(out, s2)) return 0;
  if (s3 && !obuf_puts(out, s3)) return 0;
  return 1;
}
