#define _GNU_SOURCE 1
#include <sys/types.h>
#include <sys/socket.h>

int main()
{
  struct ucred peer;
  int optlen = sizeof(peer);
  getsockopt(0, SOL_SOCKET, SO_PEERCRED, &peer, &optlen);
  return 0;
}
