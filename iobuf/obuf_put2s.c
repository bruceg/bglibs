#include <string.h>
#include "iobuf.h"

/** Write 2 C strings to the \c obuf */
int obuf_put2s(obuf* out, const char* s1, const char* s2)
{
  if (s1 && !obuf_puts(out, s1)) return 0;
  if (s2 && !obuf_puts(out, s2)) return 0;
  return 1;
}
