/* $Id$ */
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "msg/msg.h"
#include "msg/wrap.h"
#include "str/str.h"
#include "installer.h"

const char program[] = "instcheck";

static struct stat statbuf;

static str path;
static const char* makepath(const char* name)
{
  wrap_str(str_copys(&path, install_prefix));
  if (name[0] != '/')
    wrap_str(str_catc(&path, '/'));
  wrap_str(str_cats(&path, name));
  return path.s;
}

static void xchdir(int fd)
{
  if (((fd == 0) ? chdir("/") : fchdir(fd)) == -1)
    die1sys(1, "Could not change base directory");
}

static void testmode(int dir, const char* filename,
		     unsigned uid, unsigned gid, unsigned mode, unsigned type)
{
  xchdir(dir);
  if (lstat(filename, &statbuf) == -1) {
    if (errno == ENOENT)
      warnf("{Missing file: }s", filename);
    else
      diefsys(1, "{Could not stat file: }s", filename);
  }
  if ((statbuf.st_mode & S_IFMT) != type)
    warnf("{Wrong file type: }s", filename);
  if (uid != (unsigned)-1 && statbuf.st_uid != uid)
    warnf("{Wrong owner: }s", filename);
  if (gid != (unsigned)-1 && statbuf.st_gid != gid)
    warnf("{Wrong group: }s", filename);
  if (mode != (unsigned)-1 && (statbuf.st_mode & 07777) != mode)
    warnf("{Wrong permissions: }s", filename);
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
  xchdir(dir);
  if ((dir = open(subdir, O_RDONLY)) == -1)
    diefsys(1, "{Could not open directory }s", subdir);
  return dir;
}

void s(int dir, const char* name, const char* target)
{
  char linkbuf[4096];
  int linklen;
  int targetlen;
  xchdir(dir);
  testmode(dir, name, -1, -1, -1, S_IFLNK);
  if ((linklen = readlink(name, linkbuf, sizeof linkbuf)) == -1)
    diefsys(1, "{Could not read symlink: }s", name);
  targetlen = strlen(target);
  if (targetlen != linklen ||
      memcmp(linkbuf, target, linklen) != 0)
    warnf("{Wrong symlink target: }s", name);
}

int opendir(const char* dir)
{
  int fd;
  if (chdir(makepath(dir)) == -1)
    diefsys(1, "{Could not change directory: }s", path.s);
  if ((fd = open(".", O_RDONLY)) == -1)
    diefsys(1, "{Could not open directory: }s", path.s);
  return fd;
}

int opensubdir(int dir, const char* subdir)
{
  xchdir(dir);
  return opendir(subdir);
}

void instprep(void)
{
}
