/* path/match.c - Filename matching utility.
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
#include "direntry.h"
#include "str/str.h"
#include "str/iter.h"
#include "path.h"

static str tmp;

static int match_first(int absolute, const str* part, str* result)
{
  DIR* dir;
  direntry* entry;
  int count = 0;
  if (!str_truncate(result, 0)) return -1;
  if ((dir = opendir(absolute ? "/" : ".")) == 0) return -1;
  while ((entry = readdir(dir)) != 0) {
    if (entry->d_name[0] == '.') continue;
    if (fnmatch(entry->d_name, part->s)) {
      if (absolute) if (!str_catc(result, '/')) return -1;
      if (!str_catb(result, entry->d_name, strlen(entry->d_name)+1)) return -1;
      ++count;
    }
  }
  closedir(dir);
  return count;
}

static int match_next(const str* part, str* result)
{
  DIR* dir;
  direntry* entry;
  striter path;
  int count = 0;
  if (!str_copy(&tmp, result)) return -1;
  if (!str_truncate(result, 0)) return -1;
  striter_loop(&path, &tmp, 0) {
    if ((dir = opendir(path.startptr)) == 0) continue;
    while ((entry = readdir(dir)) != 0) {
      if (entry->d_name[0] == '.') continue;
      if (fnmatch(entry->d_name, part->s)) {
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

int path_match(const char* pattern, str* result)
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
  if ((count = match_first(absolute, &part, result)) == -1) return -1;
  
  for (;;) {
    pattern = partend + 1;
    while (pattern < patend && *pattern == '/') ++pattern;
    if (pattern > patend) break;
    if ((partend = strchr(pattern, '/')) == 0) partend = patend;
    if (!str_copyb(&part, pattern, partend-pattern)) return -1;
    if ((count = match_next(&part, result)) == -1) return -1;
  }
  if (!str_sort(result, 0, count)) return -1;
  return count;
}
