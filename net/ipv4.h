#ifndef NET__IPV4__H__
#define NET__IPV4__H__

#include <sysdeps.h>
typedef struct { uint8 addr[4]; } ipv4addr;
typedef uint16 ipv4port;
extern const ipv4addr IPV4ADDR_ANY;
extern const ipv4addr IPV4ADDR_BROADCAST;

extern int ipv4_parse(const char* str, ipv4addr* addr, const char** end);
extern const char* ipv4_scan(const char* str, ipv4addr* addr);
extern const char* ipv4_format(const ipv4addr* addr);

#endif
