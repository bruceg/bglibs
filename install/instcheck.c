#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "installer.h"

static struct stat statbuf;

int bin;
int man;

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

static void warn(const char* filename, const char* msg)
{
  printf("instcheck warning: File '%s' %s.\n", filename, msg);
}

static void xchdir(int fd)
{
  if (((fd == 0) ? chdir("/") : fchdir(fd)) == -1)
    diesys("Could not change base directory");
}

static void testmode(int dir, const char* filename,
		     unsigned uid, unsigned gid, unsigned mode, unsigned type)
{
  xchdir(dir);
  if (lstat(filename, &statbuf) == -1) {
    if (errno == ENOENT)
      warn(filename, "is missing");
    else
      diefsys("Could not stat file", filename);
  }
  if ((statbuf.st_mode & S_IFMT) != type)
    warn(filename, "is the wrong type of file");
  if (uid != (unsigned)-1 && statbuf.st_uid != uid)
    warn(filename, "has wrong owner");
  if (gid != (unsigned)-1 && statbuf.st_gid != gid)
    warn(filename, "has wrong group");
  if (mode != (unsigned)-1 && (statbuf.st_mode & 07777) != mode)
    warn(filename, "has wrong permissions");
}

void cf(int dir, const char* filename,
	unsigned uid, unsigned gid, unsigned mode,
	const char* srcfile)
{
  testmode(dir, filename, uid, gid, mode, S_IFREG);
  (void)srcfile;
}

void c(int dir, const char* filename,
       unsigned uid, unsigned gid, unsigned mode)
{
  cf(dir, filename, uid, gid, mode, filename);
}

int d(int dir, const char* subdir,
      unsigned uid, unsigned gid, unsigned mode)
{
  testmode(dir, subdir, uid, gid, mode, S_IFDIR);
  return opendir(subdir);
}

void s(int dir, const char* name, const char* target)
{
  char linkbuf[4096];
  int linklen;
  int targetlen;
  xchdir(dir);
  testmode(dir, name, -1, -1, -1, S_IFLNK);
  if ((linklen = readlink(name, linkbuf, sizeof linkbuf)) == -1)
    diefsys("Could not read symlink", name);
  targetlen = strlen(target);
  if (targetlen != linklen ||
      memcmp(linkbuf, target, linklen) != 0)
    warn(name, "has wrong symlink target");
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
  xchdir(dir);
  return opendir(subdir);
}

int main(void)
{
  insthier();
  return 0;
}
