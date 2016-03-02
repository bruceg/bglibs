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

/** Call a dns_* function using \c dns_qualify and save an error code if necessary. */
int resolve_qualdns(int (*dnsfn)(struct dns_transmit*, struct dns_result* out, const char*),
                    struct dns_result* out, const char* name)
{
  str fqdn = {0};
  int r = dns_qualify(out, &fqdn, name, dnsfn);
  str_free(&fqdn);
  if (r < 0) {
    __resolve_error = RESOLVE_TEMPFAIL;
    return 0;
  }
  if (out->count == 0) {
    __resolve_error = RESOLVE_NOTFOUND;
    return 0;
  }
  return 1;
}
