/* $Id$ */
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "resolve.h"

/** Look up the IPv4 address(es) corresponding to a domain name. */
int resolve_ipv4name_n(const char* name, ipv4addr* addr, int maxaddrs)
{
  struct hostent* ent;
  int i;
  h_errno = 0;
  if ((ent = gethostbyname(name)) == 0) return 0;
  if (ent->h_addrtype != AF_INET ||
      ent->h_length != 4) return 0;
  for (i = 0; i < maxaddrs && ent->h_addr_list[i] != 0; ++i)
    memcpy(&addr[i].addr, ent->h_addr_list[i], 4);
  return i;
}
