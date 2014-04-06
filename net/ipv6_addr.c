#include <string.h>

#include "ipv6.h"

/** IPv6 "any" address constant. */
const ipv6addr IPV6ADDR_ANY = {{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }};
/** IPv6 loopback address constant. */
const ipv6addr IPV6ADDR_LOOPBACK = {{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 }};
/** IPv4-in-IPv6 address prefix constant. */
const ipv6addr IPV6ADDR_V4PREFIX = {{ 0,0,0,0,0,0,0,0,0,0,0xff,0xff,0,0,0,0 }};

/** Return true if the IPv6 address is actually a wrapped IPv4 address. */
int ipv6_isv4addr(const ipv6addr* addr)
{
  return memcmp(&IPV6ADDR_V4PREFIX, addr, 12) == 0;
}
