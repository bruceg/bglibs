#include "dns.h"
#include "resolve.h"

/** Saved error code from last dns call. */
int __resolve_error = 0;

/** Call a dns_* function and save an error code if necessary. */
int resolve_calldns(int (*dnsfn)(), struct dns_result* out, const void* param)
{
  if (dnsfn(out, param) < 0) {
    __resolve_error = RESOLVE_TEMPFAIL;
    return 0;
  }
  if (out->count == 0) {
    __resolve_error = RESOLVE_NOTFOUND;
    return 0;
  }
  return 1;
}
