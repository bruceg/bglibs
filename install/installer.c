#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "installer.h"

static int sourcedir;

static char buffer[4096];

static void diesys(const char* msg)
{
  fprintf(stderr, "installer error: %s:\n  %s\n", msg,
	  strerror(errno));
  exit(1);
}

static void diefsys(const char* msg, const char* filename)
{
  fprintf(stderr, "installer error: %s '%s':\n  %s\n", msg, filename,
	  strerror(errno));
  exit(1);
}

static void setmodes(const char* filename,
		     unsigned uid, unsigned gid, unsigned mode)
{
  if (chown(filename, uid, gid) == -1)
    diefsys("Could not set owner or group for", filename);
  if (chmod(filename, mode) == -1)
    diefsys("Could not set mode for", filename);
}

void cf(int dir, const char* filename,
	unsigned uid, unsigned gid, unsigned mode,
	const char* srcfile)
{
  int fdin;
  int fdout;
  size_t rd;
  size_t wr;
  size_t offset;
  
  if (fchdir(sourcedir) == -1)
    diesys("Could not change base directory");
  if ((fdin = open(srcfile, O_RDONLY)) == -1)
    diefsys("Could not open input file", filename);

  if (fchdir(dir) == -1)
    diesys("Could not change base directory");
  if ((fdout = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0600)) == -1)
    diefsys("Could not create output file", filename);

  while ((rd = read(fdin, buffer, sizeof buffer)) != 0) {
    if (rd == (unsigned)-1) diefsys("Error reading from input file", filename);
    for (offset = 0; offset < rd; offset += wr) {
      if ((wr = write(fdout, buffer+offset, rd-offset)) == (unsigned)-1)
	diefsys("Error writing to output file", filename);
    }
  }
  if (close(fdout) == -1) diefsys("Error closing output file", filename);
  close(fdin);
  setmodes(filename, uid, gid, mode);
}

void c(int dir, const char* filename,
       unsigned uid, unsigned gid, unsigned mode)
{
  cf(dir, filename, uid, gid, mode, filename);
}

int d(int dir, const char* subdir,
      unsigned uid, unsigned gid, unsigned mode)
{
  if (fchdir(dir) == -1)
    diesys("Could not change base directory");
  if (mkdir(subdir, 0700) == -1 && errno != EEXIST)
    diefsys("Could not create directory", subdir);
  if ((dir = open(subdir, O_RDONLY)) == -1)
    diefsys("Could not open created directory", subdir);
  setmodes(subdir, uid, gid, mode);
  return dir;
}

void s(int dir, const char* name, const char* target)
{
  if (fchdir(dir) == -1)
    diesys("Could not change base directory");
  unlink(name);
  if (symlink(target, name) == -1)
    diefsys("Could not create symlink", name);
}

int opendir(const char* dir)
{
  int fd;
  if (chdir(dir) == -1)
    diefsys("Could not change directory to", dir);
  if ((fd = open(".", O_RDONLY)) == -1)
    diefsys("Could not open directory", dir);
  return fd;
}

int opensubdir(int dir, const char* subdir)
{
  if (fchdir(dir) == -1)
    diesys("Could not change base directory in opensubdir");
  return opendir(subdir);
}

int main(void)
{
  sourcedir = opendir(".");
  umask(077);
  insthier();
  return 0;
}
