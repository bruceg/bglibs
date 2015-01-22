#include "fmt.h"

/** Format an unsigned integer as decimal with padding. */
unsigned fmt_udecw(char* buffer, unsigned long num, unsigned width, char pad)
{
  return fmt_unumw(buffer, num, width, pad, 10, fmt_lcase_digits);
}

/** Format an unsigned integer as decimal. */
unsigned fmt_udec(char* buffer, unsigned long num)
{
  return fmt_unumw(buffer, num, 0, 0, 10, fmt_lcase_digits);
}
