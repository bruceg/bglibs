/* $Id$ */
/* net/recv6.c - Receive a datagram from a UDP socket
 * Copyright (C) 2004  Bruce Guenter <bruce@untroubled.org>
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

/** Receive a datagram from an IPv6 UDP socket. */
int socket_recv6(int sock, char* buffer, unsigned buflen,
		 ipv6addr* ip, ipv6port* port)
{
#if HASIPV6
  struct sockaddr_in6 sa;
  int dummy = sizeof sa;
  int r;

  r = recvfrom(sock, buffer, buflen, 0, (struct sockaddr*)&sa, &dummy);
  if (r != -1) {
    memcpy(&ip->addr, &sa.sin6_addr, 16);
    *port = ntohs(sa.sin6_port);
  }
  return r;
#else
  errno = EPROTONOSUPPORT;
  return -1;
#endif
}
