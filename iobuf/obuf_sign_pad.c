#include "iobuf.h"

int obuf_sign_pad(obuf* out, int sign, unsigned width, char pad)
{
  if (!width) return !sign || obuf_putc(out, '-');
  if (pad != '0' && !obuf_pad(out, width, pad)) return 0;
  if (sign && !obuf_putc(out, '-')) return 0;
  if (pad == '0' && !obuf_pad(out, width, pad)) return 0;
  return 1;
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
MAIN
{
  obuf_sign_pad(&outbuf, 0, 0,   0); NL();
  obuf_sign_pad(&outbuf, 1, 0,   0); NL();
  obuf_sign_pad(&outbuf, 0, 4, 'x'); NL();
  obuf_sign_pad(&outbuf, 1, 4, 'x'); NL();
  obuf_sign_pad(&outbuf, 0, 4, '0'); NL();
  obuf_sign_pad(&outbuf, 1, 4, '0'); NL();
}
#endif
#ifdef SELFTEST_EXP

-
xxxx
xxxx-
0000
-0000
#endif
