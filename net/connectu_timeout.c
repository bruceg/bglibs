/* $Id$ */
/* net/connectu_timeout.c - Make an UNIX domain connection with a timeout
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
#include "sysdeps.h"
#include <errno.h>
#include "net/socket.h"
#include "unix/nonblock.h"

/** Connect a socket to an UNIX domain address, honouring a timeout.
 *
 * \note The socket will have nonblocking mode turned on on return from
 * this function. */
int socket_connectu_timeout(int sock, const char* path, int timeout)
{
  iopoll_fd pf;
  if (!nonblock_on(sock))
    return 0;
  if (socket_connectu(sock, path))
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
