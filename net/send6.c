/* $Id$ */
/* net/send6.c - Send a datagram on a UDP socket
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
#include <string.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "socket.h"

/** Send a datagram on an IPv6 UDP socket. */
int socket_send6(int sock, const char* buffer, unsigned buflen,
		 const ipv6addr* ip, ipv6port port)
{
#if HASIPV6
  struct sockaddr_in6 sa;
  memset(&sa, 0, sizeof sa);
  sa.sin6_family = AF_INET6;
  memcpy((char*)&sa.sin6_addr, &ip->addr, 16);
  sa.sin6_port = htons(port);
  return sendto(sock, buffer, buflen, 0, (struct sockaddr*)&sa, sizeof sa);
#else
  errno = EPROTONOSUPPORT;
  return 0;
#endif
}
