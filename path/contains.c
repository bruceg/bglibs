/* path/contains.c - Filename component testing.
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

/** Test if a path contains a component.

Returns true if the \c path contains \c part either at the start
followed by a slash, at the end preceded by a slash, or elsewhere with a
slash on either side.
*/
int path_contains(const char* path, const char* part)
{
  long partlen = strlen(part);
  const char* pathend = path + strlen(path);
  while (path && path < pathend) {
    if (*path == '/')
      ++path;
    else {
      const char* slash = strchr(path, '/');
      if (!slash) slash = pathend;
      if (slash-path == partlen &&
	  memcmp(path, part, partlen) == 0)
	return 1;
      path = slash;
    }
  }
  return 0;
}
