/* net/sendfd.c - Send a file descriptor over a socket
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

/** Send a file descriptor over a socket. */
int socket_sendfd(int sock, int fd)
{
#if defined(CMSG_FIRSTHDR) && defined(MSG_NOSIGNAL) && defined(SCM_RIGHTS)
  struct msghdr msg;
  struct cmsghdr *cmsg;
  char buf[CMSG_SPACE(sizeof(int))];  /* ancillary data buffer */

  memset(&msg, 0, sizeof msg);
  msg.msg_control = buf;
  msg.msg_controllen = sizeof buf;
  cmsg = CMSG_FIRSTHDR(&msg);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  cmsg->cmsg_len = CMSG_LEN(sizeof(int));
  /* Initialize the payload: */
  *(int *)CMSG_DATA(cmsg) = fd;
  /* Sum of the length of all control messages in the buffer: */
  msg.msg_controllen = cmsg->cmsg_len;

  return sendmsg(sock, &msg, MSG_NOSIGNAL);
#else
  errno = ENOSYS;
  return -1;
#endif
}
