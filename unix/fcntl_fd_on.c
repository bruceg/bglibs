/* unix/fcntl_fd_on.c - Turn on flag with fcntl F_SETFD
 * Copyright (C) 2004  Bruce Guenter <bruceg@em.ca>
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
#include <sys/types.h>
#include <fcntl.h>
#include "fcntl.h"

int fcntl_fd_on(int fd, int flag)
{
  int flags;
  if ((flags = fcntl(fd, F_GETFD, 0)) == -1) return 0;
  if (fcntl(fd, F_SETFL, flags | flag)) return 0;
  return 1;
}
