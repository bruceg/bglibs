/* $Id$ */
/* net/send4.c - Send a datagram on a UDP socket
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

/** Send a datagram on a UDP socket. */
int socket_send4(int sock, const char* buffer, unsigned buflen,
		 const ipv4addr* ip, ipv4port port)
{
  struct sockaddr_in sa;
  memset(&sa, 0, sizeof sa);
  sa.sin_family = AF_INET;
  memcpy((char*)&sa.sin_addr, &ip->addr, 4);
  sa.sin_port = htons(port);
  return sendto(sock, buffer, buflen, 0, (struct sockaddr*)&sa, sizeof sa);
}
