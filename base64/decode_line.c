#include "base64.h"

int base64_decode_line(const char* encoded, str* bin)
{
  char tmp[3];
  int len;
  while ((len = base64_decode_part(encoded, tmp)) == 3) {
    encoded += 4;
    if (!str_catb(bin, tmp, 3)) return 0;
  }
  if (len > 0)
    return str_catb(bin, tmp, len);
  if (*encoded) return 0;
  return 1;
}
