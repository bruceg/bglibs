/* path/fnmatch.c - Filename pattern matching
 * Copyright (C) 2001,2005  Bruce Guenter <bruce@untroubled.org>
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
#include <string.h>
#include "path.h"

/** Does the string have glob characters in it.

Returns true if the given string contains at least one instance of
either <tt>*</tt>, <tt>?</tt>, or <tt>[</tt>.
*/
int has_magic(const char* s)
{
  return strchr(s, '*') || strchr(s, '?') || strchr(s, '[');
}

/** Matches a path using glob patterns. */
int fnmatch(const char* filename, const char* pattern, unsigned options)
{
  str t;
  /* Special handling for dotfiles: */
  if (filename[0] == '.') {
    /* Always skip "." and ".." */
    if (filename[1] == 0 || (filename[1] == '.' && filename[2] == 0))
      return 0;
    /* If DOTFILES is not set,
     * only match dotfiles when pattern starts with a period */
    if (!(options & PATH_MATCH_DOTFILES) && pattern[0] != '.')
      return 0;
  }
  /* Reuse the str_glob function. */
  t.s = (char*)filename;
  t.len = strlen(filename);
  t.size = 0;
  return str_globs(&t, pattern);
}

#ifdef SELFTEST_MAIN
void t(const char* filename, const char* pattern, unsigned options)
{
  obuf_puts(&outbuf, filename);
  obuf_putc(&outbuf, ' ');
  obuf_puts(&outbuf, pattern);
  obuf_putc(&outbuf, ' ');
  obuf_putu(&outbuf, options);
  obuf_putc(&outbuf, ' ');
  debugfn(fnmatch(filename, pattern, options));
}
void t1(const char* filename, const char* pattern)
{
  t(filename, pattern, 0);
  t(filename, pattern, PATH_MATCH_DOTFILES);
}
MAIN
{
  t1("abc", "*");
  t1(".", "*");
  t1("..", "*");
  t1("...", "*");
  t1(".abc", "*");
}
#endif
#ifdef SELFTEST_EXP
abc * 0 result=1
abc * 1 result=1
. * 0 result=0
. * 1 result=0
.. * 0 result=0
.. * 1 result=0
... * 0 result=0
... * 1 result=1
.abc * 0 result=0
.abc * 1 result=1
#endif
