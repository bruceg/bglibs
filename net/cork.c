/* $Id$ */
/* net/cork.c - Stop a TCP socket from sending short writes.
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
#include "socket.h"

/** Stop a TCP socket from sending short writes.

Several UNIX OS's have a facility which can prevent packets from being
sent across the wire until they are completely full, even if short
writes are sent to the socket that would normally result in output
packets.  This routine attempts to enable that facility to optimize
throughput for bulk data transfers.  It is known to work on Linux (with
the \c TCP_CORK option) and to at least compile on BSD (with the \c
TCP_NOPUSH option).  On OS's which lack either of these two options,
this function is essentially a no-op.
*/
int socket_cork(int sock)
{
#if defined(TCP_CORK)
  int flag = 1;
  return setsockopt(sock, SOL_TCP, TCP_CORK, &flag, sizeof flag) == 0;
#elif defined(TCP_NOPUSH)
  int flag = 1;
  return setsockopt(sock, SOL_SOCKET, TCP_NOPUSH, &flag, sizeof flag) == 0;
#else
  return 1;
#endif
}
