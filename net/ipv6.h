#ifndef NET__IPV6__H__
#define NET__IPV6__H__

#include <sysdeps.h>

/** \defgroup ipv6 ipv6: IPv6 Address Handling

@{ */

/** IPv6 address structure. */
typedef struct { uint8 addr[16]; } ipv6addr;
/** IPv6 port number. */
typedef uint16 ipv6port;

extern const ipv6addr IPV6ADDR_ANY;
extern const ipv6addr IPV6ADDR_LOOPBACK;

extern const char* ipv6_scan(const char* s, ipv6addr* addr);
extern const char* ipv6_format(const ipv6addr* addr);

/* @} */

#endif
