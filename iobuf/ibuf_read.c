#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "iobuf.h"

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
    if ((rd = read(io->fd, data, datalen)) == (unsigned)-1)
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
