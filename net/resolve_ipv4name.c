#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "resolve.h"

int resolve_ipv4name(const char* name, ipv4addr addr)
{
  struct hostent* ent;
  h_errno = 0;
  if ((ent = gethostbyname(name)) == 0) return 0;
  if (ent->h_addrtype != AF_INET ||
      ent->h_length != sizeof(ipv4addr)) return 0;
  memcpy(addr, ent->h_addr_list[0], sizeof(ipv4addr));
  return 1;
}
