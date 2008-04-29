#include <sys/types.h>
#include <dirent.h>

void foo()
{
  struct dirent *d;
  d->d_ino = 0;
}
