#ifndef FMT__MULTI__H__
#define FMT__MULTI__H__

#include <stdarg.h>

/** \addtogroup fmt

@{ */

/** \name Multiple item formatting

@{ */

/** Format function prototype, used by the "@" conversion in fmt_multi() */
typedef unsigned (*fmt_function)(char* buffer,
				 const void* data,
				 unsigned width,
				 char pad);

extern unsigned fmt_multi(char* buffer, const char* format, ...);
extern unsigned fmt_multiv(char* buffer, const char* format, va_list ap);

/** @} */

/** @} */

#endif
