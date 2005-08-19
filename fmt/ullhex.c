/* $Id$ */
#include "number.h"

/** Format an unsigned long long integer as hexadecimal with padding. */
unsigned fmt_ullhexw(char* buffer, unsigned long long num,
		     unsigned width, char pad)
{
  return fmt_ullnumw(buffer, num, width, pad, 16, fmt_lcase_digits);
}

/** Format an unsigned long long integer as hexadecimal. */
unsigned fmt_ullhex(char* buffer, unsigned long long num)
{
  return fmt_ullnumw(buffer, num, 0, 0, 16, fmt_lcase_digits);
}
