#include "number.h"

/** Format a signed integer as decimal with padding. */
unsigned fmt_sdecw(char* buffer, long num, unsigned width, char pad)
{
  return fmt_snumw(buffer, num, width, pad, 10, fmt_lcase_digits);
}

/** Format a signed integer as decimal. */
unsigned fmt_sdec(char* buffer, long num)
{
  return fmt_snumw(buffer, num, 0, 0, 10, fmt_lcase_digits);
}
