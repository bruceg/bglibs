/* Copyright (C) 2005  Bruce Guenter <bruce@untroubled.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */
#include "ibuf.h"
#include "str.h"

/** Read a line from the \c ibuf into a dynamic string, terminated by a CR+LF pair. */
int ibuf_getstr_crlf(ibuf* in, struct str* s)
{
  unsigned len;
  if (!ibuf_getstr(in, s, LF)) return 0;
  if (in->count == 0) return 0;
  /* Strip the trailing LF */
  len = s->len-1;
  /* Strip a trailing CRs if present */
  if (len > 0 && s->s[len-1] == CR)
    --len;
  str_truncate(s, len);
  return 1;
}

#ifdef SELFTEST_MAIN
#include <unistd.h>

ibuf in = {
  { -1, "\n\r\na\nb\r\nd", 9, 9, 0, 0, 0, 0, 0 },
  0, (ibuf_fn)read
};

MAIN
{
  static str s;
  while (ibuf_getstr_crlf(&in, &s))
    obuf_putf(&outbuf, "u{:}u{:}s{\n}", in.count, s.len, s.s);
}
#endif
#ifdef SELFTEST_EXP
1:0:
2:0:
2:1:a
3:1:b
#endif
