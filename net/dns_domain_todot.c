#include "dns.h"

int dns_domain_todot_cat(str *out,const char *d)
{
  char ch;
  char ch2;
  unsigned char ch3;
  char buf[4];

  if (!*d)
    return str_catc(out,'.');

  for (;;) {
    ch = *d++;
    while (ch--) {
      ch2 = *d++;
      if ((ch2 >= 'A') && (ch2 <= 'Z'))
	ch2 += 32;
      if (((ch2 >= 'a') && (ch2 <= 'z')) || ((ch2 >= '0') && (ch2 <= '9')) || (ch2 == '-') || (ch2 == '_')) {
        if (!str_cats(out,&ch2)) return 0;
      }
      else {
	ch3 = ch2;
	buf[3] = '0' + (ch3 & 7); ch3 >>= 3;
	buf[2] = '0' + (ch3 & 7); ch3 >>= 3;
	buf[1] = '0' + (ch3 & 7);
	buf[0] = '\\';
	if (!str_catb(out,buf,4)) return 0;
      }
    }
    if (!*d) return 1;
    if (!str_catc(out,'.')) return 0;
  }
}
