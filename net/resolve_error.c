/* $Id$ */
#include <netdb.h>
#include "resolve.h"

/** Look up the last resolve error code. */
int resolve_error(void)
{
  switch (h_errno) {
  case 0: return RESOLVE_OK;
  case HOST_NOT_FOUND: return RESOLVE_NOTFOUND;
  case NO_ADDRESS:
#ifdef NO_DATA
#if NO_DATA != NO_ADDRESS
  case NO_DATA:
#endif
#endif
    return RESOLVE_NOADDR;
  case NO_RECOVERY: return RESOLVE_NSERROR;
  case TRY_AGAIN: return RESOLVE_TEMPFAIL;
  default: return -1;
  }
}
