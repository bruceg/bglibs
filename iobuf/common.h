/* $Id$ */
#ifndef IO_BUF__COMMON__H__
#define IO_BUF__COMMON__H__

#include <fcntl.h>

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

/** @} */

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
/** @} */

/** @} */

#endif
