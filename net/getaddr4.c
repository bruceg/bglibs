/* $Id$ */
/* net/getaddr4.c - Determine the IPv4 address of a socket
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
#include <string.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "socket.h"

/** Determine the IPv4 address of a socket. */
int socket_getaddr4(int sock, ipv4addr* addr, ipv4port* port)
{
  struct sockaddr_in sa;
  int size;
  size = sizeof sa;
  if (getsockname(sock, (struct sockaddr*)&sa, &size) == -1) return 0;
  if (sa.sin_family != AF_INET) return 0;
  if (size != sizeof sa) return 0;
  memcpy(&addr->addr, &sa.sin_addr, 4);
  *port = ntohs(sa.sin_port);
  return 1;
}
