#ifndef NET__IPV4__H__
#define NET__IPV4__H__

#include <sysdeps.h>

/** \defgroup ipv4 ipv4: IPv4 Address Handling

@{ */

/** IPv4 address structure. */
typedef struct {
  /** 32-bit IPv4 address, represented as 4 8-bit \c unsigned \c chars. */
  uint8 addr[4];
} ipv4addr;
/** IPv4 port number. */
typedef uint16 ipv4port;

extern const ipv4addr IPV4ADDR_ANY;
extern const ipv4addr IPV4ADDR_BROADCAST;
extern const ipv4addr IPV4ADDR_LOOPBACK;

extern int ipv4_parse(const char* s, ipv4addr* addr, const char** end) __DEPRECATED__;
extern const char* ipv4_scan(const char* s, ipv4addr* addr);
extern const char* ipv4_format(const ipv4addr* addr);
extern unsigned fmt_ipv4addr(char* buffer, const ipv4addr* addr);

/** @} */

#endif
