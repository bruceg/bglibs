/* unix/nonblock_off.c - Turn off non-blocking I/O mode on a fd
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
#include "nonblock.h"

#ifndef O_NONBLOCK
#define O_NONBLOCK O_NDELAY
#endif

int nonblock_off(int fd)
{
  return fcntl_fl_off(fd, O_NONBLOCK);
}
