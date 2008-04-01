/* socket/notcpdelay.c - Turn on/off TCP "NODELAY" flag.
 * Copyright (C) 2003,2005  Bruce Guenter <bruce@untroubled.org>
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
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include "socket.h"

#ifndef SOL_TCP
#define SOL_TCP SOL_SOCKET
#endif

/** Turn on or off TCP NODELAY flag.
 
Enabling NODELAY disables the standard Nagle algorithm, which tries to
coalesce multiple short writes into one larger packet for better network
efficiency.  With NODELAY enabled, each write will immediately be sent
out onto the network as a seperate packet.
*/
int socket_notcpdelay(int fd, int on)
{
  return setsockopt(fd, SOL_TCP, TCP_NODELAY, &on, sizeof on) == 0;
}
