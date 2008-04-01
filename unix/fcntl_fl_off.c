/* unix/fcntl_fl_off.c - Turn off flag with fcntl F_SETFL
 * Copyright (C) 2004,2005  Bruce Guenter <bruce@untroubled.org>
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
#include <sys/types.h>
#include <fcntl.h>
#include "fcntl.h"

int fcntl_fl_off(int fd, int flag)
{
  int flags;
  int newflags;
  if ((flags = fcntl(fd, F_GETFL, 0)) == -1) return 0;
  if ((newflags = flags & ~flag) != flags)
    if (fcntl(fd, F_SETFL, newflags)) return 0;
  return 1;
}
