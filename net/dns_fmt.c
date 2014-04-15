#include "dns.h"

/** Format a DNS domain name as a dotted name. */
unsigned fmt_dns_domain(char* out, const char* domain)
{
  unsigned char ch;
  unsigned char ch2;
  char* orig;

  if (!*domain) {
    if (out)
      out[0] = '.';
    return 1;
  }
  orig = out;
  for (;;) {
    ch = *domain++;
    while (ch--) {
      ch2 = *domain++;
      if ((ch2 >= 'A') && (ch2 <= 'Z'))
	ch2 += 32;
      if (((ch2 >= 'a') && (ch2 <= 'z')) || ((ch2 >= '0') && (ch2 <= '9')) || (ch2 == '-') || (ch2 == '_')) {
	if (orig)
	  *out = ch2;
	out++;
      }
      else {
	if (orig) {
	  out[3] = '0' + (ch2 & 7); ch2 >>= 3;
	  out[2] = '0' + (ch2 & 7); ch2 >>= 3;
	  out[1] = '0' + (ch2 & 7);
	  out[0] = '\\';
	}
	out += 4;
      }
    }
    if (*domain == 0)
      break;
    if (orig)
      *out = '.';
    ++out;
  }
  return out - orig;
}
