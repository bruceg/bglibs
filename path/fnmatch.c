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

int has_magic(const char* str)
{
  return strchr(str, '*') || strchr(str, '?') || strchr(str, '[');
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
  for (; *pattern && *filename; ++pattern, ++filename) {
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
  if (start == 0) return 1;
  while (*name) {
    if (*name == start && fnmatchr1(name+1, pattern+1)) return 1;
    ++name;
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
