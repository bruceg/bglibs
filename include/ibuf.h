#ifndef IO_BUF__IBUF__H__
#define IO_BUF__IBUF__H__

#include "iobuf_common.h"

/** \defgroup ibuf ibuf: Input Buffer Managemenet

\par Calling Convention

Unless otherwise specified, the return value is non-zero (true) if the
entire requested operation completed, and \c 0 (false) otherwise.

@{ */

/** \c ibuf read function pointer prototype. */
typedef int (*ibuf_fn)(int, void*, unsigned long);

/** Input specific buffer. */
struct ibuf
{
  /** Base \c iobuf structure. */
  iobuf io;
  /** The number of bytes read in the last operation. */
  unsigned count;
  /** Function used to read data from the file descriptor. */
  ibuf_fn readfn;
};
/** Input specific buffer typedef. */
typedef struct ibuf ibuf;

extern ibuf inbuf;

int ibuf_init(ibuf* in, int fd, ibuf_fn fn, unsigned flags, unsigned bufsize);
int ibuf_open(ibuf* in, const char* filename, unsigned bufsize);
int ibuf_eof(ibuf* in);
/** Close the \c ibuf. */
#define ibuf_close(in) iobuf_close(&((in)->io))
/** Test if the \c ibuf has been closed. */
#define ibuf_closed(in) iobuf_closed(&((in)->io))
/** Test if the \c ibuf is in an error state. */
#define ibuf_error(in) iobuf_error(&((in)->io))
/** Test if the last \c ibuf read timed out. */
#define ibuf_timedout(in) iobuf_timedout(&((in)->io))
int ibuf_refill(ibuf* in);
int ibuf_read_large(ibuf* in, char* data, unsigned datalen);
int ibuf_read(ibuf* in, char* data, unsigned datalen);
unsigned ibuf_tell(ibuf* in);
int ibuf_seek(ibuf* in, unsigned offset);
/** Set the effective read position to the start of the file. */
#define ibuf_rewind(in) ibuf_seek(in,0)
/** Set the effective read position \c off bytes forward. */
#define ibuf_seekfwd(in,off) ibuf_seek(ibuf_tell(in)+(offset))

int ibuf_peek(ibuf* in, char* ch);
int ibuf_getc(ibuf* in, char* ch);
int ibuf_getu(ibuf* in, unsigned long* data);
int ibuf_gets(ibuf* in, char* data, unsigned datalen, char boundary);
int ibuf_getstr(ibuf* in, struct str* s, char boundary);
int ibuf_getstr_crlf(ibuf* in, struct str* s);
int ibuf_getnetstring(ibuf* in, struct str* s);
int ibuf_readall(ibuf* in, struct str* s);
int ibuf_openreadclose(const char* filename, struct str* s);
/** @} */

#endif
