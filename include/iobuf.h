#ifndef IO_BUF__H__
#define IO_BUF__H__

#include "iobuf_common.h"
#include "ibuf.h"
#include "obuf.h"

/** \addtogroup iobuf
 * @{ */

/** \name Mass copying functions. */

int iobuf_copy(ibuf* in, obuf* out);
int iobuf_copyflush(ibuf* in, obuf* out);
int ibuf_copytofd(ibuf* in, int out);
int obuf_copyfromfd(int in, obuf* out);

/** @} */

/** @} */

#endif
