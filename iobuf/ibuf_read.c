/* $Id$ */
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "ibuf.h"

/** Read a block of data from the \c ibuf.

This routine differs from \c ibuf_read in that the data is, as much as
is possible, read directly into the given buffer, rather than first
being read into the \c iobuf buffer and then into the given buffer.  \c
ibuf_read automatically calls this routine if the requested \c datalen
is larger than or equal to the \c iobuf buffer size. */
int ibuf_read_large(ibuf* in, char* data, unsigned datalen)
{
  iobuf* io;
  unsigned len;
  unsigned rd;
  
  in->count = 0;
  io = &(in->io);
  if (ibuf_eof(in) || ibuf_error(in)) return 0;
  
  /* If there's any content in the buffer, memcpy it out first */
  len = io->buflen - io->bufstart;
  if (len > datalen) len = datalen;
  memcpy(data, io->buffer+io->bufstart, len);
  data += len;
  datalen -= len;
  io->bufstart += len;
  in->count += len;
  
  /* After the buffer is empty and there's still more data to read,
   * read it straight from the fd instead of copying it through the buffer. */
  while (datalen) {
    if (io->timeout && !iobuf_timeout(io, 0)) return 0;
    if ((rd = in->readfn(io->fd, data, datalen)) == (unsigned)-1)
      IOBUF_SET_ERROR(io);
    if (rd == 0) {
      io->flags |= IOBUF_EOF;
      return 0;
    }
    data += rd;
    datalen -= rd;
    io->offset += rd;
    in->count += rd;
  }
  return 1;
}

/** Read a block of data from the \c ibuf

Returns true only if the entire block was read.  If the return value is
false, the caller will need to check if a partial block was read.
*/
int ibuf_read(ibuf* in, char* data, unsigned datalen)
{
  iobuf* io;
  unsigned len;
  
  io = &(in->io);
  if (datalen >= io->bufsize) return ibuf_read_large(in, data, datalen);
  in->count = 0;
  if (ibuf_eof(in) || ibuf_error(in)) return 0;
  while (datalen) {
    if (io->bufstart >= io->buflen && !ibuf_refill(in)) return 0;
    len = io->buflen - io->bufstart;
    if (len > datalen) len = datalen;
    memcpy(data, io->buffer+io->bufstart, len);
    data += len;
    datalen -= len;
    io->bufstart += len;
    in->count += len;
  }
  return 1;
}
