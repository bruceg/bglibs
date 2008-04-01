/* net/linger.c - Set the socket linger option.
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
#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include "socket.h"

/** Set the socket "linger" option. */
int socket_linger(int fd, int onoff, int seconds)
{
  struct linger opt;
  opt.l_onoff = onoff;
  opt.l_linger = seconds;
  return setsockopt(fd, SOL_SOCKET, SO_LINGER, &opt, sizeof opt) == 0;
}
