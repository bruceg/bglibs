/* net/recvfd.c - Receive a file descriptor over a socket
 * Copyright (C) 2002  Bruce Guenter <bruceg@em.ca>
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
#include <string.h>
#include <sys/socket.h>
#include "socket.h"

#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL 0
#endif

/** Receive a file descriptor over a socket. */
int socket_recvfd(int sock)
{
  char cbuf[CMSG_SPACE(sizeof(int))];
  struct msghdr msg;
  struct cmsghdr* cm;

  memset(&msg, 0, sizeof msg);
  msg.msg_control = cbuf;
  msg.msg_controllen = sizeof cbuf;
  cm = CMSG_FIRSTHDR(&msg);
  cm->cmsg_len = CMSG_LEN(sizeof(int));
  cm->cmsg_level = SOL_SOCKET;
  cm->cmsg_type = SCM_RIGHTS;
  
  if (recvmsg(sock, &msg, MSG_NOSIGNAL) == -1) return -1;
  return *(int*)CMSG_DATA(cm);
}
