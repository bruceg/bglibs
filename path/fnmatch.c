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

static int fnmatchr(const char* name, const char* pattern)
{
  char start = *pattern;
  if (start == 0) return 1;
  while (*name) {
    if (*name == start && fnmatch(name+1, pattern+1)) return 1;
    ++name;
  }
  return 0;
}

static int fnmatchs(char c, const char* set, unsigned long setlen)
{
  if (*set == '~')
    return memchr(set+1, c, setlen-1) == 0;
  else
    return memchr(set, c, setlen) != 0;
}

int fnmatch(const char* filename, const char* pattern)
{
  const char* end;
  const char* start;
  for (; *pattern && *filename; ++pattern, ++filename) {
    switch (*pattern) {
    case '?': continue;
    case '*':
      return fnmatchr(filename, pattern+1);
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
