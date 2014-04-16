#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "dns.h"

/** Free a DNS result structure. */
void dns_result_free(struct dns_result* d)
{
  int i;
  if (d->rr.__ptr != 0) {
    switch(d->type) {
    case DNS_T_MX:
      for (i = 0; i < d->count; ++i)
	if (d->rr.mx[i].name)
	  free(d->rr.mx[i].name);
      break;
    case DNS_T_PTR:
    case DNS_T_TXT:
      for (i = 0; i < d->count; ++i)
	if (d->rr.name[i])
	  free(d->rr.name[i]);
      break;
    }
    free(d->rr.__ptr);
    d->rr.__ptr = 0;
  }
}

/** Allocate a DNS result structure. */
int dns_result_alloc(struct dns_result* d, int type, int count)
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
  if (d->count != count || d->type != type) {
    dns_result_free(d);
    if (count > 0) {
      if ((d->rr.__ptr = malloc(count * size)) == 0) {
	errno = ENOMEM;
	return -1;
      }
      memset(d->rr.__ptr, 0, count * size);
    }
    d->count = count;
    d->type = type;
  }
  return 0;
}
