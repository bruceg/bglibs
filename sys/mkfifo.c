#include "sysdeps.h"
#ifndef HASMKFIFO
#include <sys/types.h>
#include <sys/stat.h>

int mkfifo(const char* filename, int mode)
{
  return mknod(filename, (mode & 07777) | S_IFIFO, 0);
}
#endif
