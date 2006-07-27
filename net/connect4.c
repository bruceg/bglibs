/* $Id$ */
/* net/connect4.c - Make an IPv4 connection
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
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "socket.h"

/** Connect a socket to an IPv4 address. */
int socket_connect4(int sock, const ipv4addr* ip, ipv4port port)
{
  struct sockaddr_in sa;
  memset(&sa, 0, sizeof sa);
  sa.sin_family = AF_INET;
  memcpy((char*)&sa.sin_addr, &ip->addr, 4);
  sa.sin_port = htons(port);
  return connect(sock, (struct sockaddr*)&sa, sizeof sa) == 0;
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
MAIN
{
  int sock1;
  int sock2;
  ipv4port port = 12345;
  debugsys(sock1 = socket_tcp4());
  debugsys(socket_reuse(sock1));
  for (port = 12345; port < 32768; ++port)
    if (socket_bind4(sock1, &IPV4ADDR_ANY, port) == 0)
      break;
  debugsys(socket_listen(sock1, 1));
  debugsys(sock2 = socket_tcp4());
  debugsys(socket_connect4(sock2, &IPV4ADDR_LOOPBACK, port));
}
#endif
#ifdef SELFTEST_EXP
result=3
result=1
result=1
result=4
result=0
#endif
