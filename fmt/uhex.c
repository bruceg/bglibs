#include "number.h"

/** Format an unsigned integer as (lower-case) hexadecimal with
    padding. */
unsigned fmt_uhexw(char* buffer, unsigned long num, unsigned width, char pad)
{
  return fmt_unumw(buffer, num, width, pad, 16, fmt_lcase_digits);
}

/** Format an unsigned integer as (lower-case) hexadecimal. */
unsigned fmt_uhex(char* buffer, unsigned long num)
{
  return fmt_unumw(buffer, num, 0, 0, 16, fmt_lcase_digits);
}
