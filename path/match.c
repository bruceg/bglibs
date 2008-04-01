/* path/match.c - Filename matching utility.
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
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

#include "sysdeps.h"
#include "str/str.h"
#include "str/iter.h"
#include "path.h"

static str tmplist;
static str tmpitem;

static int exists(const char* path)
{
  struct stat st;
  if (stat(path, &st) == 0) return 1;
  if (errno == ENOENT || errno == EACCES || errno == ENOTDIR) return 0;
  return -1;
}

static int match_first(int absolute, const str* part, str* result,
		       unsigned options)
{
  DIR* dir;
  direntry* entry;
  int count = 0;
  if (!str_truncate(result, 0)) return -1;
  if (has_magic(part->s)) {
    if ((dir = opendir(absolute ? "/" : ".")) == 0) return -1;
    while ((entry = readdir(dir)) != 0) {
      if (fnmatch(entry->d_name, part->s, options)) {
	if (absolute) if (!str_catc(result, '/')) return -1;
	if (!str_catb(result, entry->d_name, strlen(entry->d_name)+1))
	  return -1;
	++count;
      }
    }
    closedir(dir);
    return count;
  }
  else {
    switch (exists(part->s)) {
    case 1: str_copyb(result, part->s, part->len + 1); return 1;
    case 0: return 0;
    default: return -1;
    }
  }
}

static int match_next_magic(const str* part, str* result, unsigned options)
{
  DIR* dir;
  direntry* entry;
  striter path;
  int magic;
  int count;
  count = 0;
  magic = has_magic(part->s);
  if (!str_copy(&tmplist, result)) return -1;
  if (!str_truncate(result, 0)) return -1;
  striter_loop(&path, &tmplist, 0) {
    if ((dir = opendir(path.startptr)) == 0) continue;
    while ((entry = readdir(dir)) != 0) {
      if (fnmatch(entry->d_name, part->s, options)) {
	if (!str_cats(result, path.startptr) ||
	    !str_catc(result, '/') ||
	    !str_catb(result, entry->d_name, strlen(entry->d_name)+1)) {
	  closedir(dir);
	  return -1;
	}
	++count;
      }
    }
    closedir(dir);
  }
  return count;
}

static int match_next_nomagic(const str* part, str* result, unsigned options)
{
  striter path;
  int count = 0;
  if (!fnmatch(part->s, part->s, options)) return 0;
  if (!str_copy(&tmplist, result)) return -1;
  if (!str_truncate(result, 0)) return -1;
  striter_loop(&path, &tmplist, 0) {
    if (!str_copyiter(&tmpitem, &path) ||
	!str_catc(&tmpitem, '/') ||
	!str_cat(&tmpitem, part)) return -1;
    switch (exists(tmpitem.s)) {
    case 1: str_catb(result, tmpitem.s, tmpitem.len+1); ++count; continue;
    case 0: continue;
    default: return -1;
    }
  }
  return count;
}

static int match_next(const str* part, str* result, unsigned options)
{
  return has_magic(part->s) ?
    match_next_magic(part, result, options) :
    match_next_nomagic(part, result, options);
}

/** Matches the pattern against existing files.

This function produces a list of existing files in \c result (in the
current directory) that match the given pattern.  If \c options has \c
PATH_MATCH_DOTFILES set, the result may contain paths starting with ".".
*/
int path_match(const char* pattern, str* result, unsigned options)
{
  static str part;
  
  int count;
  const char* patend;
  int absolute;
  const char* partend;
  
  absolute = pattern[0] == '/';
  patend = pattern + strlen(pattern);
  /* Note and skip any leading slashes */
  while (pattern < patend && *pattern == '/') ++pattern;
  if ((partend = strchr(pattern, '/')) == 0) partend = patend;

  if (!str_copyb(&part, pattern, partend-pattern)) return -1;
  if ((count = match_first(absolute, &part, result, options)) == -1) return -1;
  
  while (result->len > 0) {
    pattern = partend + 1;
    while (pattern < patend && *pattern == '/') ++pattern;
    if (pattern > patend) break;
    if ((partend = strchr(pattern, '/')) == 0) partend = patend;
    if (!str_copyb(&part, pattern, partend-pattern)) return -1;
    if ((count = match_next(&part, result, options)) == -1) return -1;
  }
  if (!str_sort(result, 0, count, 0)) return -1;
  str_free(&tmplist);
  return count;
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
static void match(const char* pattern, unsigned options)
{
  static str s;
  striter i;
  obuf_puts(&outbuf, pattern);
  obuf_putc(&outbuf, ' ');
  obuf_putu(&outbuf, options);
  obuf_puts(&outbuf, " => ");
  obuf_puti(&outbuf, path_match(pattern, &s, options));
  NL();
  for (striter_start(&i, &s, 0);
       striter_valid(&i);
       striter_advance(&i)) {
    obuf_putiter(&outbuf, &i);
    NL();
  }
}
MAIN
{
  match("*", 0);
  match("*.o", 0);
  match("*.o*", 0);
  match("*.[eo]", 0);
  match("t*", 0);
  match("*.[!o]*", 0);
}
#endif
#ifdef SELFTEST_EXP
* 0 => 4
test
test.exp
test.o
test.out
*.o 0 => 1
test.o
*.o* 0 => 2
test.o
test.out
*.[eo] 0 => 1
test.o
t* 0 => 4
test
test.exp
test.o
test.out
*.[!o]* 0 => 1
test.exp
#endif
