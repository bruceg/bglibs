#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "client.h"

int vclient_req_write(const vclient_req* vr, int fd)
{
  unsigned wr;
  unsigned char buf[VCLIENT_MAXLEN+3];
  unsigned total = vr->data.len + 1;
  unsigned char* ptr = buf;
  
  *ptr++ = 2;
  *ptr++ = (unsigned char)(total >> 8);
  *ptr++ = (unsigned char)total;
  *ptr++ = vr->args;
  memcpy(ptr, vr->data.s, vr->data.len);
  for (ptr = buf; total > 0; ptr += wr, total -= wr) {
    if ((wr = write(fd, ptr, total)) == (unsigned)-1)
      if (errno != EINTR) return 0;
  }
  return 1;
}
