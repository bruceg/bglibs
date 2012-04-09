#define opendir orig_opendir
#include "sysdeps.h"
#undef opendir
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "iobuf/obuf.h"
#include "msg/msg.h"
#include "msg/wrap.h"
#include "str/str.h"
#include "installer.h"

const char program[] = "instshow";

#define DENTRY_SLOTS 250
static str dentries[DENTRY_SLOTS];
static int last_dentry = 1;

static void showmode(char type, unsigned mode)
{
  obuf_putf(&outbuf, "cccccccccc",
	    type,
	    (mode & 0400) ? 'r' : '-',
	    (mode & 0200) ? 'w' : '-',
	    (mode & 0100)
	    ? (mode & 04000) ? 's' : 'x'
	    : (mode & 04000) ? 'S' : '-',
	    (mode & 040) ? 'r' : '-',
	    (mode & 020) ? 'w' : '-',
	    (mode & 010)
	    ? (mode & 02000) ? 's' : 'x'
	    : (mode & 02000) ? 'S' : '-',
	    (mode & 04) ? 'r' : '-',
	    (mode & 02) ? 'w' : '-',
	    (mode & 01)
	    ? (mode & 01000) ? 't' : 'x'
	    : (mode & 01000) ? 'T' : '-');
}

#define UID_NA "      N/A"

static void show(char type, int dir, const char* filename,
		 unsigned uid, unsigned gid, unsigned mode,
		 const char* endline)
{
  showmode(type, mode);

  if (uid == (unsigned)-1)
    obuf_puts(&outbuf, UID_NA);
  else
    obuf_putf(&outbuf, "\\ 8d", uid);
  if (gid == (unsigned)-1)
    obuf_puts(&outbuf, UID_NA);
  else
    obuf_putf(&outbuf, "\\ 8d", gid);

  obuf_putc(&outbuf, ' ');
  if (filename[0] != '/') {
    obuf_puts(&outbuf, install_prefix);
    if (dentries[dir].s[0] != '/')
      obuf_putc(&outbuf, '/');
    obuf_putstr(&outbuf, &dentries[dir]);
    obuf_putc(&outbuf, '/');
  }
  obuf_puts(&outbuf, filename);
  obuf_puts(&outbuf, endline);
}

void c(int dir, const char* filename,
       unsigned uid, unsigned gid, unsigned mode)
{
  show('-', dir, filename, uid, gid, mode, "\n");
}

void cf(int dir, const char* filename,
	unsigned uid, unsigned gid, unsigned mode,
	const char* srcfile)
{
  /*
  show('-', dir, filename, uid, gid, mode, " <= ");
  obuf_puts(&outbuf, srcfile);
  obuf_putc(&outbuf, '\n');
  */
  c(dir, filename, uid, gid, mode);
  (void)srcfile;
}

int d(int dir, const char* subdir,
      unsigned uid, unsigned gid, unsigned mode)
{
  show('d', dir, subdir, uid, gid, mode, "\n");
  return opensubdir(dir, subdir);
}

void s(int dir, const char* name, const char* target)
{
  show('l', dir, name, -1, -1, 0777, " -> ");
  obuf_puts(&outbuf, target);
  obuf_putc(&outbuf, '\n');
}

int opendir(const char* dir)
{
  if (last_dentry >= DENTRY_SLOTS)
    die1(1, "Too many open directories");
  wrap_str(str_copys(&dentries[last_dentry], dir));
  return last_dentry++;
}

int opensubdir(int dir, const char* subdir)
{
  if (last_dentry >= DENTRY_SLOTS)
    die1(1, "Too many open directories");
  wrap_str(str_copy(&dentries[last_dentry], &dentries[dir]));
  if (dir > 0)
    wrap_str(str_catc(&dentries[last_dentry], '/'));
  wrap_str(str_cats(&dentries[last_dentry], subdir));
  return last_dentry++;
}

void instprep(void)
{
  dentries[0].s = "";
  dentries[0].len = 0;
  obuf_puts(&outbuf, " type/mode    owner    group path\n");
}
