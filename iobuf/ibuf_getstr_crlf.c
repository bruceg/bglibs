#include "iobuf.h"
#include "str/str.h"

/** Read a line from the \c ibuf into a dynamic string, terminated by a CR+LF pair. */
int ibuf_getstr_crlf(ibuf* in, struct str* s)
{
  unsigned len;
  if (!ibuf_getstr(in, s, LF)) return 0;
  if (inbuf.count == 0) return 0;
  /* Strip the trailing LF */
  len = s->len-1;
  /* Strip a trailing CRs if present */
  if (s->s[len-1] == CR) --len;
  str_truncate(s, len);
  return 1;
}
