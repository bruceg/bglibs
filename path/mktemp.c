#include <fcntl.h>
#include <sys/stat.h>
#include "systime.h"
#include <sys/types.h>
#include <unistd.h>
#include "path.h"

int path_mktemp(const char* prefix, str* filename)
{
  struct stat s;
  long pid = getpid();
  struct timeval tv;
  do {
    if (!str_copys(filename, prefix)) return -1;
    if (!str_catc(filename, '.')) return -1;
    if (!str_catu(filename, pid)) return -1;
    gettimeofday(&tv, 0);
    if (!str_catc(filename, '.')) return -1;
    if (!str_catu(filename, tv.tv_sec)) return -1;
    if (!str_catc(filename, '.')) return -1;
    if (!str_catu(filename, tv.tv_usec)) return -1;
  } while (lstat(filename->s, &s) != -1);
  return open(filename->s, O_RDWR | O_EXCL | O_CREAT, 0600);
}
