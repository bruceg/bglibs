#include "fmt.h"

/** Format an unsigned long long integer as (upper-case) hexadecimal
    with padding. */
unsigned fmt_ullHexw(char* buffer, unsigned long long num,
		     unsigned width, char pad)
{
  return fmt_ullnumw(buffer, num, width, pad, 16, fmt_ucase_digits);
}

/** Format an unsigned long long integer as (upper-case) hexadecimal. */
unsigned fmt_ullHex(char* buffer, unsigned long long num)
{
  return fmt_ullnumw(buffer, num, 0, 0, 16, fmt_ucase_digits);
}
