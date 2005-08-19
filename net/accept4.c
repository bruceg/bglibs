/* $Id$ */
/* net/accept4.c - Accept an IPv4 connection
 * Copyright (C) 2001  Bruce Guenter <bruceg@em.ca>
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
  int dummy;
  int fd;

  dummy = sizeof sa;
  if ((fd = accept(sock, (struct sockaddr*)&sa, &dummy)) != -1) {
    memcpy(&ip->addr, (char*)&sa.sin_addr, 4);
    *port = ntohs(sa.sin_port);
  }
  return fd;
}
