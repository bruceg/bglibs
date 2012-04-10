#define opendir orig_opendir
#include "sysdeps.h"
#undef opendir
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "msg.h"
#include "wrap.h"
#include "path.h"
#include "str.h"
#include "installer.h"

const char program[] = "installer";

static int sourcedir;

static char buffer[4096];

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

static void setmodes(const char* filename,
		     unsigned uid, unsigned gid, unsigned mode)
{
  if (chown(filename, uid, gid) == -1)
    diefsys(1, "{Could not set owner or group for }s", filename);
  if (chmod(filename, mode) == -1)
    diefsys(1, "{Could not set mode for }s", filename);
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
  static str tmpname;
  
  xchdir(sourcedir);
  if ((fdin = open(srcfile, O_RDONLY)) == -1)
    diefsys(1, "{Could not open input file }s", srcfile);

  xchdir(dir);
  if ((fdout = path_mktemp(filename, &tmpname)) == -1)
    die1sys(1, "Could not create temporary file");

  while ((rd = read(fdin, buffer, sizeof buffer)) != 0) {
    if (rd == (unsigned)-1)
      diefsys(1, "{Error reading from input file }s", filename);
    for (offset = 0; offset < rd; offset += wr) {
      if ((wr = write(fdout, buffer+offset, rd-offset)) == (unsigned)-1)
	die1sys(1, "Error writing to output file");
    }
  }
  if (close(fdout) == -1)
    diefsys(1, "{Error closing output file }s", tmpname.s);
  setmodes(tmpname.s, uid, gid, mode);
  if (rename(tmpname.s, filename))
    diefsys(1, "{Could not rename '}s{' to '}s{'}", tmpname.s, filename);
  close(fdin);
}

void c(int dir, const char* filename,
       unsigned uid, unsigned gid, unsigned mode)
{
  cf(dir, filename, uid, gid, mode, filename);
}

int d(int dir, const char* subdir,
      unsigned uid, unsigned gid, unsigned mode)
{
  xchdir(dir);
  if (path_mkdirs(subdir, 0700) == -1 && errno != EEXIST)
    diefsys(1, "{Could not create directory }s", subdir);
  if ((dir = open(subdir, O_RDONLY)) == -1)
    diefsys(1, "{Could not open created directory }s", subdir);
  setmodes(subdir, uid, gid, mode);
  return dir;
}

void s(int dir, const char* name, const char* target)
{
  xchdir(dir);
  unlink(name);
  if (symlink(target, name) == -1)
    diefsys(1, "{Could not create symlink }s", name);
}

static int dochdir(const char* dir)
{
  int fd;
  if (chdir(dir) == -1)
    diefsys(1, "{Could not change directory to }s", dir);
  if ((fd = open(".", O_RDONLY)) == -1)
    diefsys(1, "{Could not open directory }s", dir);
  return fd;
}

int opendir(const char* dir)
{
  dir = makepath(dir);
  if (path_mkdirs(dir, 0777) == -1
      && errno != EEXIST)
    diefsys(1, "{Could not create directory }s", dir);
  return dochdir(dir);
}

int opensubdir(int dir, const char* subdir)
{
  xchdir(dir);
  return dochdir(subdir);
}

void instprep(void)
{
  if ((sourcedir = open(".", O_RDONLY)) == -1)
    die1sys(1, "Could not open working directory");
  umask(077);
}
