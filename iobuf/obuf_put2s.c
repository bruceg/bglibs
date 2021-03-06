#include <string.h>
#include "obuf.h"

/** Write 2 C strings to the \c obuf */
int obuf_put2s(obuf* out, const char* s1, const char* s2)
{
  if (s1 && !obuf_puts(out, s1)) return 0;
  if (s2 && !obuf_puts(out, s2)) return 0;
  return 1;
}

#ifdef SELFTEST_MAIN
MAIN
{
  obuf_put2s(&outbuf, "1", "2"); NL();
  obuf_put2s(&outbuf,   0, "2"); NL();
  obuf_put2s(&outbuf, "1",   0); NL();
}
#endif
#ifdef SELFTEST_EXP
12
2
1
#endif
