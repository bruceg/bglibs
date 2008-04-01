#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "client.h"

int vclient_resp_read(vclient_resp* vr, int fd)
{
  unsigned char buf[3];
  unsigned length;
  char* ptr;
  unsigned rd;
  
  if (read(fd, buf, 3) != 3) return 0;
  vr->code = buf[0];
  length = buf[1] << 8 | buf[2];
  if (!str_truncate(&vr->message, length)) return 0;
  for (ptr = vr->message.s; length > 0; ptr += rd, length -= rd) {
    if ((rd = read(fd, ptr, length)) == (unsigned)-1)
      if (errno != EINTR) return 0;
  }
  vr->message.len = length;
  return 1;
}
