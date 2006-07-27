/* $Id$ */
/* net/accept6.c - Accept an IPv6 connection
 * Copyright (C) 2006  Bruce Guenter <bruce@untroubled.org>
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
#include <string.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "socket.h"

/** Accept a connection on a socket from an IPv6 address. */
int socket_accept6(int sock, ipv6addr* ip, ipv6port *port)
{
#if HASIPV6
  struct sockaddr_in6 sa;
  socklen_t dummy;
  int fd;

  dummy = sizeof sa;
  if ((fd = accept(sock, (struct sockaddr*)&sa, &dummy)) != -1) {
    memcpy(&ip->addr, (char*)&sa.sin6_addr, 16);
    *port = ntohs(sa.sin6_port);
  }
  return fd;
#else
  errno = EPROTONOSUPPORT;
  return -1;
#endif
}
