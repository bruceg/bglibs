#include "sysdeps.h"

#if ! HASGETPEEREID

#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>

int getpeereid(int s, uid_t* u, gid_t* g)
{
#if HASPEERCRED
  struct ucred peer;
  socklen_t optlen = sizeof peer;
  if (getsockopt(s, SOL_SOCKET, SO_PEERCRED, &peer, &optlen) == -1)
    return -1;
  if (u != 0)
    *u = peer.uid;
  if (g != 0)
    *g = peer.gid;
  return 0;
#else
  errno = ENOSYS;
  return -1;
  s = 0; u = 0; g = 0;
#endif
}

#endif /* HASGETPEEREID */
