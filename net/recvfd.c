/* net/recvfd.c - Receive a file descriptor over a socket
 * Copyright (C) 2002,2005  Bruce Guenter <bruce@untroubled.org>
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
#include <errno.h>
#include <string.h>
#include "cmsg.h"
#include "socket.h"

/** Receive a file descriptor over a socket. */
int socket_recvfd(int sock)
{
#if defined(CMSG_FIRSTHDR) && defined(MSG_NOSIGNAL) && defined(SCM_RIGHTS)
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
#else
  errno = ENOSYS;
  return -1;
#endif
}
