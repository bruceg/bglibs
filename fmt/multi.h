/* $Id$ */
#ifndef FMT__MULTI__H__
#define FMT__MULTI__H__

#include <stdarg.h>

/** \addtogroup fmt

@{ */

/** \name Multiple item formatting

@{ */

extern unsigned fmt_multi(char* buffer, const char* format, ...);
extern unsigned fmt_multiv(char* buffer, const char* format, va_list ap);

/** @} */

/** @} */

#endif
