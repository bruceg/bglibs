#ifndef NET__RESOLVE__H__
#define NET__RESOLVE__H__

#include "ipv4.h"

#define RESOLVE_OK 0
#define RESOLVE_NOTFOUND 1
#define RESOLVE_NOADDR 2
#define RESOLVE_NSERROR 3
#define RESOLVE_TEMPFAIL 4

extern const char* resolve_ipv4addr(const ipv4addr* addr);
extern int resolve_ipv4name(const char* name, ipv4addr* addr);
extern int resolve_error(void);

#endif
