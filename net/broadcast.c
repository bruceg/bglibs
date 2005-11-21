/* $Id$ */
/* net/broadcast.c - Mark a socket for broadcast transmission.
 * Copyright (C) 2001  Bruce Guenter <bruce@untroubled.org>
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
#include <sys/param.h>
#include <sys/socket.h>
#include "socket.h"

/** Mark a socket for broadcast transmission. */
int socket_broadcast(int fd)
{
  int opt = 1;
  return setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &opt, sizeof opt) == 0;
}
