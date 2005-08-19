/* $Id$ */
#include "sysdeps.h"
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include "path.h"

/** Create a directory, optionally creating any parent directories.

This function creates the named directory using <tt>mkdir</tt>.  If any
of the parent directories of the specified path do not exist, they are
created as well (with the same mode).

\note Unlike the "<tt>mkdir -p</tt>" command, this function will result
in an error if the path already exists.
*/
int path_mkdirs(const char* path, unsigned mode)
{
  int i;
  i = strlen(path);
  while (i > 0 && path[i-1] == '/')
    --i;
  while (i > 0 && path[i-1] != '/')
    --i;
  while (i > 0 && path[i-1] == '/')
    --i;
  if (i > 0) {
    struct stat st;
    char prefix[i+1];
    memcpy(prefix, path, i);
    prefix[i] = 0;
    if (stat(prefix, &st) == 0) {
      if (!S_ISDIR(st.st_mode)) {
	errno = ENOTDIR;
	return -1;
      }
    }
    else if (errno != ENOENT)
      return -1;
    else if (path_mkdirs(prefix, mode) != 0)
      return -1;
  }
  return mkdir(path, mode);
}
