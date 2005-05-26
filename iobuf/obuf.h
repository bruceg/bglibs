#ifndef IO_BUF__OBUF__H__
#define IO_BUF__OBUF__H__

#include <iobuf/common.h>

/** \defgroup obuf obuf: Output Buffer Management

\par Calling Convention

Unless otherwise specified, the return value is non-zero (true) if the
entire requested operation completed, and \c 0 (false) otherwise.

@{ */

/** \c obuf write function pointer prototype. */
typedef int (*obuf_fn)(int, const void*, unsigned long);

/** Output specific buffer. */
struct obuf
{
  /** Base \c iobuf structure. */
  iobuf io;
  /** Current write position within the buffer. */
  unsigned bufpos;
  /** The number of bytes written in the last operation. */
  unsigned count;
  /** Function used to write data to the file descriptor. */
  obuf_fn writefn;
};
typedef struct obuf obuf;

extern obuf outbuf;
extern obuf errbuf;

extern const char obuf_dec_digits[10];
extern const char obuf_hex_lcase_digits[16];
extern const char obuf_hex_ucase_digits[16];

/** Create the file if it does not already exist. */
#define OBUF_CREATE O_CREAT
/** If \c OBUF_CREATE is set, fail to create the file if it already exists. */
#define OBUF_EXCLUSIVE O_EXCL
/** If the file exists, truncate it to length 0. */
#define OBUF_TRUNCATE O_TRUNC
/** All writes go to the end of the file. */
#define OBUF_APPEND O_APPEND

int obuf_init(obuf* out, int fd, obuf_fn fn, unsigned flags, unsigned bufsize);
int obuf_open(obuf* out, const char* filename, int oflags, int mode, unsigned bufsize);
int obuf_close(obuf* out);
/** Test if the \c obuf is in an error state. */
#define obuf_error(out) iobuf_error(&(out)->io)
/** Test if the \c obuf has been closed. */
#define obuf_closed(out) iobuf_closed(&(out)->io)
/** Test if the last \c obuf write timed out. */
#define obuf_timedout(out) iobuf_timedout(&((out)->io))
int obuf_flush(obuf* out);
int obuf_sync(obuf* out);
int obuf_write_large(obuf* out, const char* data, unsigned datalen);
int obuf_write(obuf* out, const char* data, unsigned datalen);
int obuf_seek(obuf* out, unsigned offset);
/** Set the effective write position to the start of the file. */
#define obuf_rewind(out) obuf_seek(out,0)
/** Look up the current effective write position. */
#define obuf_tell(out) ((out)->io.offset+(out)->bufpos)

int obuf_pad(obuf* out, unsigned width, char ch);
int obuf_endl(obuf* out);
int obuf_putc(obuf* out, char ch);
/** Write a C string to the \c obuf */
#define obuf_puts(out,str) obuf_write(out,str,strlen(str))
int obuf_put2s(obuf* out, const char* s1, const char* s2);
int obuf_put3s(obuf* out, const char* s1, const char* s2, const char* s3);
int obuf_put4s(obuf* out, const char* s1, const char* s2, const char* s3,
	       const char* s4);
int obuf_put5s(obuf* out, const char* s1, const char* s2, const char* s3,
	       const char* s4, const char* s5);
int obuf_put6s(obuf* out, const char* s1, const char* s2, const char* s3,
	       const char* s4, const char* s5, const char* s6);
int obuf_put7s(obuf* out, const char* s1, const char* s2, const char* s3,
	       const char* s4, const char* s5, const char* s6, const char* s7);
int obuf_putns(obuf* out, unsigned int count, ...);
int obuf_putf(obuf* out, const char* format, ...);
/** Write a dynamic string to the \c iobuf */
#define obuf_putstr(out,str) obuf_write(out,(str)->s,(str)->len)
int obuf_putsflush(obuf* out, const char* s);
int obuf_puti(obuf* out, long data);
int obuf_putiw(obuf* out, long data, unsigned width, char pad);
int obuf_putu(obuf* out, unsigned long data);
int obuf_putuw(obuf* out, unsigned long data, unsigned width, char pad);
int obuf_putill(obuf* out, long long data);
int obuf_putiwll(obuf* out, long long data, unsigned width, char pad);
int obuf_putull(obuf* out, unsigned long long data);
int obuf_putuwll(obuf* out, unsigned long long data, unsigned width, char pad);
int obuf_putx(obuf* out, unsigned long data);
int obuf_putxw(obuf* out, unsigned long data, unsigned width, char pad);
int obuf_putX(obuf* out, unsigned long data);
int obuf_putXw(obuf* out, unsigned long data, unsigned width, char pad);
int obuf_putxll(obuf* out, unsigned long long data);
int obuf_putxwll(obuf* out, unsigned long long data, unsigned width, char pad);
int obuf_putXll(obuf* out, unsigned long long data);
int obuf_putXwll(obuf* out, unsigned long long data, unsigned width, char pad);
int obuf_putsnumw(obuf* out, long num, unsigned width, char pad,
		  unsigned base, const char* digits);
int obuf_putunumw(obuf* out, unsigned long num, unsigned width, char pad,
		  unsigned base, const char* digits);
int obuf_putsllnumw(obuf* out, long long num, unsigned width, char pad,
		    unsigned base, const char* digits);
int obuf_putullnumw(obuf* out, unsigned long long num, unsigned width, char pad,
		    unsigned base, const char* digits);
int obuf_putnetstring(obuf* out, const char* data, unsigned datalen);
int obuf_sign_pad(obuf* out, int sign, unsigned width, char pad);
/* @} */

#endif
