#include <errno.h>
#include <unistd.h>
#include "iobuf.h"

int ibuf_read_large(ibuf* in, char* data, unsigned datalen)
{
  iobuf* io;
  unsigned len;
  unsigned rd;
  
  in->count = 0;
  io = &(in->io);
  
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
  while (datalen > 0) {
    if (io->timeout && !iobuf_timeout(io, 0)) return 0;
    rd = read(io->fd, data, datalen);
    if (rd == -1)
      IOBUF_SET_ERROR(io);
    else if (rd == 0) {
      io->flags |= IOBUF_EOF;
      break;
    }
    data += rd;
    datalen -= rd;
    io->offset += rd;
    in->count += rd;
  }
  return datalen == 0;
}

int ibuf_read(ibuf* in, char* data, unsigned datalen)
{
  iobuf* io;
  unsigned len;
  
  io = &(in->io);
  if (datalen >= io->bufsize) return ibuf_read_large(in, data, datalen);
  in->count = 0;
  while (datalen) {
    if (io->bufstart >= io->buflen) ibuf_refill(in);
    if (ibuf_eof(in) || ibuf_error(in) || ibuf_timedout(in)) return 0;
    len = io->buflen - io->bufstart;
    if (len > datalen) len = datalen;
    memcpy(data, io->buffer+io->bufstart, len);
    data += len;
    datalen -= len;
    io->bufstart += len;
    in->count += len;
  }
  return datalen == 0;
}
