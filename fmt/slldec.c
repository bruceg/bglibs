#include "number.h"

/** Format a signed long long integer as decimal with padding. */
unsigned fmt_slldecw(char* buffer, long long num, unsigned width, char pad)
{
  return fmt_sllnumw(buffer, num, width, pad, 10, fmt_lcase_digits);
}

/** Format a signed long long integer as decimal. */
unsigned fmt_slldec(char* buffer, long long num)
{
  return fmt_sllnumw(buffer, num, 0, 0, 10, fmt_lcase_digits);
}
