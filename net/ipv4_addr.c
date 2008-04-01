#include "ipv4.h"

/** IPv4 "any" address constant. */
const ipv4addr IPV4ADDR_ANY = {{ 0,0,0,0 }};
/** IPv4 broadcast address constant. */
const ipv4addr IPV4ADDR_BROADCAST = {{ 255,255,255,255 }};
/** IPv4 loopback address constant. */
const ipv4addr IPV4ADDR_LOOPBACK = {{ 127,0,0,1 }};
