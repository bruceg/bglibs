/* $Id$ */
#include "base64.h"

void base64_encode_whole(const unsigned char bin[3], char encoded[4])
{
  encoded[0] = base64_bin2asc[bin[0] >> 2];
  encoded[1] = base64_bin2asc[(bin[0] << 4 | bin[1] >> 4) & 0x3f];
  encoded[2] = base64_bin2asc[(bin[1] << 2 | bin[2] >> 6) & 0x3f];
  encoded[3] = base64_bin2asc[bin[2] & 0x3f];
}

void base64_encode_part(const unsigned char bin[3], unsigned len,
			char encoded[4])
{
  encoded[0] = base64_bin2asc[bin[0] >> 2];
  /* len must be 1 or 2, as 0 produces nothing, and 3 would be handled
   * by _whole */
  if (len == 1) {
    encoded[1] = base64_bin2asc[(bin[0] << 4) & 0x3f];
    encoded[2] = encoded[3] = BASE64_PAD;
  }
  else {
    encoded[1] = base64_bin2asc[(bin[0] << 4 | bin[1] >> 4) & 0x3f];
    encoded[2] = base64_bin2asc[(bin[1] << 2) & 0x3f];
    encoded[3] = BASE64_PAD;
  }
}
