/* socket/notcpdelay.c - Turn on/off TCP "NODELAY" flag.
 * Copyright (C) 2003  Bruce Guenter <bruceg@em.ca>
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
#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include "socket.h"

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
