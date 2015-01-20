#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "dns.h"

/** Free a DNS result structure. */
void dns_result_free(struct dns_result* d)
{
  if (d->rr.__ptr != 0) {
    free(d->rr.__ptr);
    d->rr.__ptr = 0;
  }
  if (d->__buffer != 0) {
    free(d->__buffer);
    d->__buffer = 0;
  }
}

/** Allocate a DNS result structure. */
int dns_result_alloc(struct dns_result* d, int type, int count, int bufsize)
{
  int size;
  switch (type) {
  case DNS_T_A:
    size = sizeof(ipv4addr); break;
  case DNS_T_AAAA:
    size = sizeof(ipv6addr); break;
  case DNS_T_MX:
    size = sizeof(struct dns_mx); break;
  case DNS_T_PTR:
  case DNS_T_TXT:
    size = sizeof(void*); break;
  default:
    errno = EPROTO;
    return -1;
  }
  dns_result_free(d);
  if (count > 0) {
    if ((d->rr.__ptr = malloc(count * size)) == 0) {
      errno = ENOMEM;
      return -1;
    }
    memset(d->rr.__ptr, 0, count * size);
    if (bufsize > 0 && (d->__buffer = malloc(bufsize)) == 0) {
      errno = ENOMEM;
      return -1;
    }
  }
  d->count = count;
  d->type = type;
  return 0;
}
