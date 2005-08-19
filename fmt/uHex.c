/* $Id$ */
#include "number.h"

/** Format an unsigned integer as (upper-case) hexadecimal with
    padding. */
unsigned fmt_uHexw(char* buffer, unsigned long num, unsigned width, char pad)
{
  return fmt_unumw(buffer, num, width, pad, 16, fmt_ucase_digits);
}

/** Format an unsigned integer as (upper-case) hexadecimal. */
unsigned fmt_uHex(char* buffer, unsigned long num)
{
  return fmt_unumw(buffer, num, 0, 0, 16, fmt_ucase_digits);
}
