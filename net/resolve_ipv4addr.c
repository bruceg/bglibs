#include <netdb.h>
#include <sys/socket.h>
#include "resolve.h"

const char* resolve_ipv4addr(const ipv4addr addr)
{
  struct hostent* ent;
  h_errno = 0;
  if ((ent = gethostbyaddr(addr, sizeof(ipv4addr), AF_INET)) == 0) return 0;
  return ent->h_name;
}
