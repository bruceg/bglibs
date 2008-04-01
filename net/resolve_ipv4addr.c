#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "resolve.h"

/** Look up the domain name corresponding to an IPv4 address. */
const char* resolve_ipv4addr(const ipv4addr* addr)
{
  struct hostent* ent;
  h_errno = 0;
  if ((ent = gethostbyaddr(&addr->addr, 4, AF_INET)) == 0) return 0;
  return ent->h_name;
}
