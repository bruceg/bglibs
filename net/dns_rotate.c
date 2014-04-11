#include <string.h>

#include "dns.h"

/** Rotate (shuffle) a block of fixed-length addresses.

    \param s The array of memory to shuffle.
    \param n The number of elements of the array.
    \param shift The size of each element, expressed as a power-of-two
*/
void dns_rotate(unsigned char *s, unsigned int n, unsigned int shift)
{
  unsigned int i;
  unsigned int stride = 1 << shift;
  unsigned char tmp[stride];

  while (n > 1) {
    i = dns_random(n);
    --n;
    memcpy(tmp, s + (i << shift), stride);
    memcpy(s + (i << shift), s + (n << shift), stride);
    memcpy(s + (n << shift), tmp, stride);
  }
}

/** Rotate (shuffle) a block of IPv4 addresses. */
void dns_rotateipv4(ipv4addr* addrs, unsigned int n)
{
  dns_rotate((unsigned char*)addrs, n, 2);
}

/** Rotate (shuffle) a block of IPv6 addresses. */
void dns_rotateipv6(ipv6addr* addrs, unsigned int n)
{
  dns_rotate((unsigned char*)addrs, n, 4);
}
