#ifndef NET__IPV4__H__
#define NET__IPV4__H__

typedef unsigned char ipv4addr[4];
typedef unsigned short ipv4port;
extern ipv4addr IPV4ADDR_ANY;
extern ipv4addr IPV4ADDR_BROADCAST;

extern int ipv4_parse(const char* str, ipv4addr addr, const char** end);
extern const char* ipv4_format(const ipv4addr addr);

#endif
