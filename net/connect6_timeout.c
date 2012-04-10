/* net/connect6_timeout.c - Make an IPv6 connection with a timeout
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
#include "sysdeps.h"
#include <errno.h>
#include "socket.h"
#include "nonblock.h"

/** Connect a socket to an IPv6 address, honouring a timeout.
 *
 * \note The socket will have nonblocking mode turned on on return from
 * this function. */
int socket_connect6_timeout(int sock, const ipv6addr* ip, ipv6port port,
			    int timeout)
{
  iopoll_fd pf;
  if (!nonblock_on(sock))
    return 0;
  if (socket_connect6(sock, ip, port))
    return 1;
  if (errno != EINPROGRESS && errno != EWOULDBLOCK)
    return 0;
  pf.fd = sock;
  pf.events = IOPOLL_WRITE;
  switch (iopoll_restart(&pf, 1, timeout)) {
  case 0:
    errno = ETIMEDOUT;
    return 0;
  case 1:
    if (socket_connected(sock))
      return 1;
    /* No break, fall through */
  default:
    return 0;
  }
}
