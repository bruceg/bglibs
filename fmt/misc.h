/* $Id$ */
#ifndef FMT__MISC__H__
#define FMT__MISC__H__

#include <stdarg.h>
struct str;

/** \addtogroup fmt

@{ */

/** \name Miscelaneous formatting

@{ */

extern unsigned fmt_char(char* buffer, int ch, unsigned width, char pad);
extern unsigned fmt_mem(char* buffer, const char* s, unsigned length,
			unsigned width, char pad);
extern unsigned fmt_chars(char* buffer, const char* s,
			  unsigned width, char pad);
extern unsigned fmt_str(char* buffer, const struct str* s,
			unsigned width, char pad);

/** @} */

/** @} */

#endif
