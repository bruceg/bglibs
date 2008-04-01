#include <string.h>
#include "obuf.h"

/** Write 4 C strings to the \c obuf */
int obuf_put4s(obuf* out, const char* s1, const char* s2, const char* s3,
	       const char* s4)
{
  if (s1 && !obuf_puts(out, s1)) return 0;
  if (s2 && !obuf_puts(out, s2)) return 0;
  if (s3 && !obuf_puts(out, s3)) return 0;
  if (s4 && !obuf_puts(out, s4)) return 0;
  return 1;
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
MAIN
{
  obuf_put4s(&outbuf, "1", "2", "3", "4"); NL();
  obuf_put4s(&outbuf,   0, "2", "3", "4"); NL();
  obuf_put4s(&outbuf, "1",   0, "3", "4"); NL();
  obuf_put4s(&outbuf, "1", "2",   0, "4"); NL();
  obuf_put4s(&outbuf, "1", "2", "3",   0); NL();
}
#endif
#ifdef SELFTEST_EXP
1234
234
134
124
123
#endif
