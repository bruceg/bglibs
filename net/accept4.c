/* $Id$ */
/* net/accept4.c - Accept an IPv4 connection
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
#include <string.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "socket.h"

/** Accept a connection on a socket from an IPv4 address. */
int socket_accept4(int sock, ipv4addr* ip, ipv4port *port)
{
  struct sockaddr_in sa;
  socklen_t dummy;
  int fd;

  dummy = sizeof sa;
  if ((fd = accept(sock, (struct sockaddr*)&sa, &dummy)) != -1) {
    memcpy(&ip->addr, (char*)&sa.sin_addr, 4);
    *port = ntohs(sa.sin_port);
  }
  return fd;
}
