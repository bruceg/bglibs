#ifndef IO_BUF__H__
#define IO_BUF__H__

#define LF ((char)10)
#define CR ((char)13)
#define CRLF "\015\012"

struct str;

/** \defgroup iobuf iobuf: I/O Buffer Management

\par Calling Convention

Unless otherwise specified, the return value is non-zero (true) if the
entire requested operation completed, and \c 0 (false) otherwise.

@{ */

/** \name Status Flags
 * @{ */

/** Reading reached end of file. */
#define IOBUF_EOF 1
/** The iobuf has encountered an error. */
#define IOBUF_ERROR 2
/** The timeout expired before the read or write could be completed. */
#define IOBUF_TIMEOUT 4
/** Mask of all the error type flags. */
#define IOBUF_BADFLAGS 0xf
/** \c lseek is possible on the file descriptor. */
#define IOBUF_SEEKABLE 0x10
/** The file descriptor needs to be closed. */
#define IOBUF_NEEDSCLOSE 0x20
/** The buffer needs to be deallocated with \c free */
#define IOBUF_NEEDSFREE 0x40
/** The buffer needs to be deallocated with \c munmap */
#define IOBUF_NEEDSMUNMAP 0x80
extern unsigned iobuf_bufsize;

/* @} */

/** \name Common
 * @{ */

/** The base I/O buffer structure. */
struct iobuf
{
  /** File descriptor being buffered. */
  int fd;
  /** The buffer memory. */
  char* buffer;
  /** Total buffer size. */
  unsigned bufsize;
  /** Length of the data in the buffer. */
  unsigned buflen;
  /** Start of the data in the buffer. */
  unsigned bufstart;
  /** Current file read/write offset. */
  unsigned offset;
  /** I/O timeout in ms (\c 0 for no timeout). */
  unsigned timeout;
  /** Status flags. */
  unsigned flags;
  /** Saved errno flag. */
  int errnum;
};
typedef struct iobuf iobuf;

/** Set the error flag, save \c errno, and return false. */
#define IOBUF_SET_ERROR(io) \
do{ \
  io->flags |= IOBUF_ERROR; \
  io->errnum = errno; \
  return 0; \
}while(0)

int iobuf_init(iobuf* io, int fd, unsigned bufsize, char* buffer,
	       unsigned flags);
int iobuf_close(iobuf* io);
/** True if the \c iobuf has been closed. */
#define iobuf_closed(io) ((io)->fd == -1)
/** True if the \c iobuf has an error flag. */
#define iobuf_error(io) ((io)->flags & IOBUF_ERROR)
/** True if the input or output to the \c iobuf previously timed out. */
#define iobuf_timedout(io) ((io)->flags & IOBUF_TIMEOUT)
/** True if the \c iobuf is in a "bad" state. */
#define iobuf_bad(io) ((io)->flags & IOBUF_BADFLAGS)
int iobuf_timeout(iobuf* io, int poll_out);
/* @} */

/** \name ibuf: Input
 * @{ */

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
/* @} */

/** \name obuf: Output
 * @{ */

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

#include <fcntl.h>
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
/** Write a dynamic string to the \c iobuf */
#define obuf_putstr(out,str) obuf_write(out,(str)->s,(str)->len)
int obuf_putsflush(obuf* out, const char* s);
int obuf_putiw(obuf* out, long data, unsigned width, char pad);
#define obuf_puti(out,data) obuf_putiw(out, data, 0, 0)
int obuf_putuw(obuf* out, unsigned long data, unsigned width, char pad);
#define obuf_putu(out,data) obuf_putuw(out, data, 0, 0)
int obuf_putiwll(obuf* out, long long data, unsigned width, char pad);
#define obuf_putill(out,data) obuf_putiw(out, data, 0, 0)
int obuf_putuwll(obuf* out, unsigned long long data, unsigned width, char pad);
#define obuf_putull(out,data) obuf_putuw(out, data, 0, 0)
int obuf_putxw(obuf* out, unsigned long data, unsigned width, char pad);
#define obuf_putx(out,data) obuf_putxw(out, data, 0, 0)
int obuf_putnetstring(obuf* out, const char* data, unsigned datalen);
/* @} */

/** \name */

int iobuf_copy(ibuf* in, obuf* out);
int iobuf_copyflush(ibuf* in, obuf* out);

/* @} */

#endif
