#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "dns.h"

/** Return the length of an encoded domain name. */
unsigned int dns_domain_length(const char *dn)
{
  const char *x;
  unsigned char c;

  x = dn;
  while ((c = *x++) != 0)
    x += (unsigned int) c;
  return x - dn;
}

/** Free a pointer to a domain name. */
void dns_domain_free(char **out)
{
  if (*out) {
    free(*out);
    *out = 0;
  }
}

/** Copy an encoded domain name to a new pointer. */
int dns_domain_copy(char **out,const char *in)
{
  unsigned int len;
  char *x;

  len = dns_domain_length(in);
  x = malloc(len);
  if (!x) return 0;
  memcpy(x,in,len);
  if (*out) free(*out);
  *out = x;
  return 1;
}

/** Compare two domain names for equality. */
int dns_domain_equal(const char *dn1,const char *dn2)
{
  unsigned int len;

  len = dns_domain_length(dn1);
  if (len != dns_domain_length(dn2)) return 0;

  if (strncasecmp(dn1,dn2,len)) return 0; /* safe since 63 < 'A' */
  return 1;
}

/** Test if the domain name \p big ends with \p little. */
int dns_domain_suffix(const char *big,const char *little)
{
  unsigned char c;

  for (;;) {
    if (dns_domain_equal(big,little)) return 1;
    c = *big++;
    if (!c) return 0;
    big += c;
  }
}

/** Determine the location of the suffix \c little within \c big . */
unsigned int dns_domain_suffixpos(const char *big,const char *little)
{
  const char *orig = big;
  unsigned char c;

  for (;;) {
    if (dns_domain_equal(big,little)) return big - orig;
    c = *big++;
    if (!c) return 0;
    big += c;
  }
}
