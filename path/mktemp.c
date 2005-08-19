/* $Id$ */
#include <fcntl.h>
#include <sys/stat.h>
#include "systime.h"
#include <sys/types.h>
#include <unistd.h>
#include "path.h"

/** Create a temporary file.

This function creates a temporary file by adding an difficult-to-predict
suffix (using the PID and microsecond timestamp) to the given prefix.
If this filename does not exist, it is opened in read/write mode.
*/
int path_mktemp(const char* prefix, str* filename)
{
  struct stat s;
  static long pid = 0;
  struct timeval tv;
  if (pid == 0)
    pid = getpid();
  do {
    if (!str_copys(filename, prefix)) return -1;
    if (!str_catc(filename, '.')) return -1;
    if (!str_catu(filename, pid)) return -1;
    gettimeofday(&tv, 0);
    if (!str_catc(filename, '.')) return -1;
    if (!str_catu(filename, tv.tv_sec)) return -1;
    if (!str_catc(filename, '.')) return -1;
    if (!str_catuw(filename, tv.tv_usec, 6, '0')) return -1;
  } while (lstat(filename->s, &s) != -1);
  return open(filename->s, O_RDWR | O_EXCL | O_CREAT, 0600);
}
