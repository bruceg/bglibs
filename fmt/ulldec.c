/* $Id$ */
#include "number.h"

/** Format an unsigned long long integer as decimal with padding. */
unsigned fmt_ulldecw(char* buffer, unsigned long long num,
		     unsigned width, char pad)
{
  return fmt_ullnumw(buffer, num, width, pad, 10, fmt_lcase_digits);
}

/** Format an unsigned long long integer as decimal. */
unsigned fmt_ulldec(char* buffer, unsigned long long num)
{
  return fmt_ullnumw(buffer, num, 0, 0, 10, fmt_lcase_digits);
}
