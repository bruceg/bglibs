#include "base64.h"

int base64_decode_part(const char encoded[4], unsigned char bin[3])
{
  int e0;
  int e1;
  int e2;
  int e3;
  if (encoded[0] == BASE64_PAD) return 0;
  if ((e0 = base64_asc2bin[(unsigned char)encoded[0]]) == -1 ||
      (e1 = base64_asc2bin[(unsigned char)encoded[1]]) == -1) return -1;
  bin[0] = (e0 << 2) | (e1 >> 4);
  if (encoded[2] == BASE64_PAD) return 1;
  if ((e2 = base64_asc2bin[(unsigned char)encoded[2]]) == -1) return -1;
  bin[1] = (e1 << 4) | (e2 >> 2);
  if (encoded[3] == BASE64_PAD) return 2;
  if ((e3 = base64_asc2bin[(unsigned char)encoded[3]]) == -1) return -1;
  bin[2] = (e2 << 6) | e3;
  return 3;
}
