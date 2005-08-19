/* $Id$ */
#ifndef FMT__NUMBER__H__
#define FMT__NUMBER__H__

/** \defgroup fmt fmt: Formatting Functions

\par Function Naming

-# Prefix
   - \c fmt_ : Always present
-# Data type
   - \c s : Signed integer (long)
   - \c u : Unsigned integer (long)
   - \c sll : Signed long long integer
   - \c ull : Unsigned long long integer
-# Conversion type
   - \c num : Generic number
   - \c dec : Decimal
   - \c hex : Hexadecimal (lower case)
   - \c Hex : Hexadecimal (upper case)
-# Width / Padding
   - \c w : (optional) Pad out the result to a minimum width

For example, \c fmt_sdecw formats a signed integer using decimal with
width padding.

\par Calling Convention

Parameters to the fmt functions are passed in the following order.
The presence of all parameters, except for \c buffer, is dependant on
which function is being used.

-# \c buffer : The character string into which to put the data result.
   If this is \c NULL, no data is written, and only the length is
   calculated.  This is useful for determining how long a formatted
   string might be.
-# \c number or \c data : The input data item to convert.
-# \c width : The minimum output width.
-# \c pad : The character with which to pad the output.
-# \c base : The numerical base to use.
-# \c digits : The array of digits to use.

\par Return Value

All fmt functions return the number of bytes written to the buffer
space.

@{ */

/** The maximum space used by a formatted number.  This value is long
  enough for 2^128 plus a trailing \c NUL byte. */
#define FMT_ULONG_LEN 40

extern const char fmt_lcase_digits[36];
extern const char fmt_ucase_digits[36];

extern unsigned fmt_pad(char* buffer, unsigned width, char pad);
extern unsigned fmt_sign_pad(char* buffer, int sign,
			     unsigned width, char pad);

/** \name Signed integer conversions
 * @{ */

extern unsigned fmt_snumw(char* buffer, long num,
			  unsigned width, char pad,
			  unsigned base, const char* digits);
extern unsigned fmt_sdec(char* buffer, long num);
extern unsigned fmt_sdecw(char* buffer, long num,
			  unsigned width, char pad);

/** @} */

/** \name Signed long long integer conversions
 * @{ */

extern unsigned fmt_sllnumw(char* buffer, long long num,
			    unsigned width, char pad,
			    unsigned base, const char* digits);
extern unsigned fmt_slldec(char* buffer, long long num);
extern unsigned fmt_slldecw(char* buffer, long long num,
			    unsigned width, char pad);

/** @} */

/** \name Unsigned integer conversions
 * @{ */

extern unsigned fmt_unumw(char* buffer, unsigned long num,
			  unsigned width, char pad,
			  unsigned base, const char* digits);
extern unsigned fmt_udec(char* buffer, unsigned long num);
extern unsigned fmt_udecw(char* buffer, unsigned long num,
			  unsigned width, char pad);
extern unsigned fmt_uhex(char* buffer, unsigned long num);
extern unsigned fmt_uhexw(char* buffer, unsigned long num,
			  unsigned width, char pad);
extern unsigned fmt_uHex(char* buffer, unsigned long num);
extern unsigned fmt_uHexw(char* buffer, unsigned long num,
			  unsigned width, char pad);

/** @} */

/** \name Unsigned long long integer conversions
 * @{ */

extern unsigned fmt_ullnumw(char* buffer, unsigned long long num,
			    unsigned width, char pad,
			    unsigned base, const char* digits);
extern unsigned fmt_ulldec(char* buffer, unsigned long long num);
extern unsigned fmt_ulldecw(char* buffer, unsigned long long num,
			    unsigned width, char pad);
extern unsigned fmt_ullhex(char* buffer, unsigned long long num);
extern unsigned fmt_ullhexw(char* buffer, unsigned long long num,
			    unsigned width, char pad);
extern unsigned fmt_ullHex(char* buffer, unsigned long long num);
extern unsigned fmt_ullHexw(char* buffer, unsigned long long num,
			    unsigned width, char pad);

/** @} */

/** @} */

#endif
