#include <stdlib.h>
#include <string.h>
#include "ucspi.h"

static const char* proto;
static unsigned protolen;
static char* vartmp;

#define MAXNAME 20

const char* ucspi_protocol(void)
{
  if (proto == 0 && (proto = getenv("PROTO")) != 0) {
    protolen = strlen(proto);
    vartmp = malloc(protolen + MAXNAME+1);
    memcpy(vartmp, proto, protolen);
    memset(vartmp + protolen, 0, MAXNAME+1);
  }
  return proto;
}

const char* ucspi_getenv(const char* name)
{
  if (ucspi_protocol() == 0)
    return 0;
  strncpy(vartmp + protolen, name, MAXNAME);
  return getenv(vartmp);
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
void put(const char* s)
{
  obuf_puts(&outbuf, s);
  NL();
}
MAIN
{
  setenv("PROTO", "xyzzy", 1);
  setenv("xyzzyLOCALIP", "1.2.3.4", 1);
  setenv("xyzzyREMOTEIP", "5.6.7.8", 1);
  setenv("xyzzyLOCALHOST", "1.host.net", 1);
  setenv("xyzzyREMOTEHOST", "2.host.net", 1);
  setenv("xyzzy0123456789012345678", "19", 1);
  setenv("xyzzy01234567890123456789", "20", 1);
  setenv("xyzzy012345678901234567890", "21", 1);
  setenv("xyzzy012345678901234567890123456789", "30", 1);
  put(ucspi_protocol());
  put(ucspi_localip());
  put(ucspi_remoteip());
  put(ucspi_localhost());
  put(ucspi_remotehost());
  put(ucspi_getenv("012345678901234567890123456789"));
}
#endif
#ifdef SELFTEST_EXP
xyzzy
1.2.3.4
5.6.7.8
1.host.net
2.host.net
20
#endif
