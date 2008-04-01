/* net/uncork.c - Unstop a TCP socket from sending short writes.
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
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <unistd.h>
#include "socket.h"

/** Unstop a TCP socket from sending short writes. */
int socket_uncork(int sock)
{
#if defined(TCP_CORK)
  int flag = 0;
  return setsockopt(sock, SOL_TCP, TCP_CORK, &flag, sizeof flag) == 0;
#elif defined(TCP_NOPUSH)
  /* BSD's TCP_NOPUSH option only takes effect after a write/send.
   * Hopefully this extra write will cause the TCP stack to send out
   * any buffered writes. */
  int flag = 0;
  if (setsockopt(sock, SOL_SOCKET, TCP_NOPUSH, &flag, sizeof flag) != 0) return 0;
  return write(sock, &flag, 0) == 0;
#else
  return 1;
#endif
}
