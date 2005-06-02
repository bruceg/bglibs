#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "msg/msg.h"
#include "installer.h"

const char program[] = "installer";
const int msg_show_pid = 0;

static int sourcedir;

static char buffer[4096];

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
  
  xchdir(sourcedir);
  if ((fdin = open(srcfile, O_RDONLY)) == -1)
    diefsys(1, "{Could not open input file }s", srcfile);

  xchdir(dir);
  if ((fdout = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0600)) == -1)
    diefsys(1, "{Could not create output file }s", filename);

  while ((rd = read(fdin, buffer, sizeof buffer)) != 0) {
    if (rd == (unsigned)-1)
      diefsys(1, "{Error reading from input file }s", filename);
    for (offset = 0; offset < rd; offset += wr) {
      if ((wr = write(fdout, buffer+offset, rd-offset)) == (unsigned)-1)
	diefsys(1, "{Error writing to output file }s", filename);
    }
  }
  if (close(fdout) == -1)
    diefsys(1, "{Error closing output file }s", filename);
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
  xchdir(dir);
  if (mkdir(subdir, 0700) == -1 && errno != EEXIST)
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

int opendir(const char* dir)
{
  int fd;
  if (chdir(dir) == -1)
    diefsys(1, "{Could not change directory to }s", dir);
  if ((fd = open(".", O_RDONLY)) == -1)
    diefsys(1, "{Could not open directory }s", dir);
  return fd;
}

int opensubdir(int dir, const char* subdir)
{
  xchdir(dir);
  return opendir(subdir);
}

int main(void)
{
  sourcedir = opendir(".");
  umask(077);
  insthier();
  return 0;
}
