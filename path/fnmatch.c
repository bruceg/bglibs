/* path/fnmatch.c - Filename pattern matching
 * Copyright (C) 2001  Bruce Guenter <bruceg@em.ca>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include <string.h>
#include "path.h"

/* GLOB patterns:
 * *      matches everything
 * ?      matches any single character
 * [seq]  matches any sing echaracter in seq
 * [~seq] matches any single character not in seq
 */

static int is_magic(char c)
{
  return c == '*' || c == '?' || c == '[';
}

int has_magic(const char* s)
{
  return strchr(s, '*') || strchr(s, '?') || strchr(s, '[');
}

static int fnmatchs(char c, const char* set, unsigned long setlen)
{
  if (*set == '~')
    return memchr(set+1, c, setlen-1) == 0;
  else
    return memchr(set, c, setlen) != 0;
}

static int fnmatchr2(const char* name, const char* pattern);
static int fnmatchr1(const char* filename, const char* pattern)
{
  const char* end;
  const char* start;
  for (; *pattern; ++pattern, ++filename) {
    if (*filename == 0)
      return pattern[0] == '*' && pattern[1] == 0;
    switch (*pattern) {
    case '?': continue;
    case '*':
      return fnmatchr2(filename, pattern+1);
    case '[':
      start = pattern+1;
      if ((end = strchr(start, ']')) == 0) return -1;
      if (!fnmatchs(*filename, start, end-start)) return 0;
      pattern = end;
      break;
    default:
      if (*pattern != *filename) return 0;
    }
  }
  return *filename == 0 && *pattern == 0;
}

static int fnmatchr2(const char* name, const char* pattern)
{
  char start = *pattern;
  int r;
  if (start == 0) return 1;
  if (is_magic(start)) {
    for (; *name != 0; ++name)
      if ((r = fnmatchr1(name, pattern)) != 0)
	return r;
  }
  else {
    ++pattern;
    while (*name)
      if (*name++ == start && (r = fnmatchr1(name, pattern)) != 0)
	return r;
  }
  return 0;
}

int fnmatch(const char* filename, const char* pattern, unsigned options)
{
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
  return fnmatchr1(filename, pattern);
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
void t(const char* pattern, unsigned options)
{
  obuf_puts(&outbuf, pattern);
  obuf_putc(&outbuf, ' ');
  obuf_putu(&outbuf, options);
  obuf_putc(&outbuf, ' ');
  debugfn(fnmatch("abc", pattern, options));
}
MAIN
{
  t("*", 0);
  t("**", 0);
  /* Three patterns for each case:
   * 1 at start of filename
   * 2 in filename
   * 3 at end of filename
   * Four characters to test:
   * 1 First filename character "a"
   * 2 Second filename character "b"
   * 3 Third filename character "c"
   * 4 Non-filename character "x"
   */
  /* First case: single character */
  t("a*", 0);
  t("*a*", 0);
  t("*a", 0);
  t("b*", 0);
  t("*b*", 0);
  t("*b", 0);
  t("c*", 0);
  t("*c*", 0);
  t("*c", 0);
  t("x*", 0);
  t("*x*", 0);
  t("*x", 0);
  /* Second case: character in a set */
  t("[axy]*", 0);
  t("*[axy]*", 0);
  t("*[axy]", 0);
  t("[bxy]*", 0);
  t("*[bxy]*", 0);
  t("*[bxy]", 0);
  t("[cxy]*", 0);
  t("*[cxy]*", 0);
  t("*[cxy]", 0);
  t("[xyz]*", 0);
  t("*[xyz]*", 0);
  t("*[xyz]", 0);
  /* Third case: negated set */
  t("[~a]*", 0);
  t("*[~a]*", 0);
  t("*[~a]", 0);
  t("[~b]*", 0);
  t("*[~b]*", 0);
  t("*[~b]", 0);
  t("[~c]*", 0);
  t("*[~c]*", 0);
  t("*[~c]", 0);
  t("[~x]*", 0);
  t("*[~x]*", 0);
  t("*[~x]", 0);
  /* Fourth case: preceding wildcard */
  t("?a*", 0);
  t("*?a*", 0);
  t("*?a", 0);
  t("?b*", 0);
  t("*?b*", 0);
  t("*?b", 0);
  t("?c*", 0);
  t("*?c*", 0);
  t("*?c", 0);
  t("?x*", 0);
  t("*?x*", 0);
  t("*?x", 0);
  /* Fifth case: following wildcard */
  t("a?*", 0);
  t("*a?*", 0);
  t("*a?", 0);
  t("b?*", 0);
  t("*b?*", 0);
  t("*b?", 0);
  t("c?*", 0);
  t("*c?*", 0);
  t("*c?", 0);
  t("x?*", 0);
  t("*x?*", 0);
  t("*x?", 0);
}
#endif
#ifdef SELFTEST_EXP
* 0 result=1
** 0 result=1
a* 0 result=1
*a* 0 result=1
*a 0 result=0
b* 0 result=0
*b* 0 result=1
*b 0 result=0
c* 0 result=0
*c* 0 result=1
*c 0 result=1
x* 0 result=0
*x* 0 result=0
*x 0 result=0
[axy]* 0 result=1
*[axy]* 0 result=1
*[axy] 0 result=0
[bxy]* 0 result=0
*[bxy]* 0 result=1
*[bxy] 0 result=0
[cxy]* 0 result=0
*[cxy]* 0 result=1
*[cxy] 0 result=1
[xyz]* 0 result=0
*[xyz]* 0 result=0
*[xyz] 0 result=0
[~a]* 0 result=0
*[~a]* 0 result=1
*[~a] 0 result=1
[~b]* 0 result=1
*[~b]* 0 result=1
*[~b] 0 result=1
[~c]* 0 result=1
*[~c]* 0 result=1
*[~c] 0 result=0
[~x]* 0 result=1
*[~x]* 0 result=1
*[~x] 0 result=1
?a* 0 result=0
*?a* 0 result=0
*?a 0 result=0
?b* 0 result=1
*?b* 0 result=1
*?b 0 result=0
?c* 0 result=0
*?c* 0 result=1
*?c 0 result=1
?x* 0 result=0
*?x* 0 result=0
*?x 0 result=0
a?* 0 result=1
*a?* 0 result=1
*a? 0 result=0
b?* 0 result=0
*b?* 0 result=1
*b? 0 result=1
c?* 0 result=0
*c?* 0 result=0
*c? 0 result=0
x?* 0 result=0
*x?* 0 result=0
*x? 0 result=0
#endif
