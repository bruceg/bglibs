#include "base64.h"

int base64_encode_line(const unsigned char* bin, unsigned long len,
		       str* encoded)
{
  char tmp[4];
  while (len >= 3) {
    base64_encode_whole(bin, tmp);
    if (!str_catb(encoded, tmp, 4)) return 0;
    bin += 3;
    len -= 3;
  }
  if (len) {
    base64_encode_part(bin, len, tmp);
    if (!str_catb(encoded, tmp, 4)) return 0;
  }
  return 1;
}
