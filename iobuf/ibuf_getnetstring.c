#include <errno.h>
#include "ibuf.h"
#include "str/str.h"

/** Read a netstring from the given \c ibuf .

If the input is not a netstring, \c errno (and \c in->io.errnum ) will
be set to \c EPROTOTYPE and the state of \c in will be undefined.
*/
int ibuf_getnetstring(ibuf* in, struct str* s)
{
  unsigned long len;
  char ch;
  if (!ibuf_getu(in, &len)) return 0;
  if (!ibuf_getc(in, &ch)) return 0;
  if (ch != ':') {
    errno = in->io.errnum = EPROTOTYPE;
    return 0;
  }
  if (!str_ready(s, len)) return 0;
  if (!ibuf_read(in, s->s, len)) return 0;
  s->s[s->len = len] = 0;
  if (!ibuf_getc(in, &ch)) return 0;
  if (ch != ',') {
    errno = in->io.errnum = EPROTOTYPE;
    return 0;
  }
  return 1;
}
