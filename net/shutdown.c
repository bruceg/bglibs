/* $Id$ */
/* net/shutdown.c - Shutdown part of a socket.
 * Copyright (C) 2001  Bruce Guenter <bruce@untroubled.org>
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
#include "socket.h"

#ifndef SHUT_RD
#define SHUT_RD 0
#endif

#ifndef SHUT_WR
#define SHUT_WR 1
#endif

#ifndef SHUT_RDWR
#define SHUT_RDWR 2
#endif

/** Shut down part or all of a socket. */
int socket_shutdown(int sock, int shut_rd, int shut_wr)
{
  int how;
  if (shut_rd && shut_wr) how = SHUT_RDWR;
  else if (shut_rd) how = SHUT_RD;
  else if (shut_wr) how = SHUT_WR;
  else return 1;
  return shutdown(sock, how) == 0;
}
