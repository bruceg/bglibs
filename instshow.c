#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "installer.h"

#define DENTRY_SLOTS 250
static const char* dentries[DENTRY_SLOTS] = {0};
static int last_dentry = 0;

static void showmode(char type, unsigned mode)
{
  putchar(type);
  putchar((mode & 0400) ? 'r' : '-');
  putchar((mode & 0200) ? 'w' : '-');
  putchar((mode & 0100) ?
	  (mode & 04000) ? 's' : 'x' :
	  (mode & 04000) ? 'S' : '-');
  putchar((mode & 040) ? 'r' : '-');
  putchar((mode & 020) ? 'w' : '-');
  putchar((mode & 010) ?
	  (mode & 02000) ? 's' : 'x' :
	  (mode & 02000) ? 'S' : '-');
  putchar((mode & 04) ? 'r' : '-');
  putchar((mode & 02) ? 'w' : '-');
  putchar((mode & 01) ?
	  (mode & 01000) ? 't' : 'x' :
	  (mode & 01000) ? 'T' : '-');
}

#define UID_NA "      N/A"

static void show(char type, int dir, const char* filename,
		 unsigned uid, unsigned gid, unsigned mode,
		 const char* endline)
{
  showmode(type, mode);

  if (uid == (unsigned)-1) fputs(UID_NA, stdout);
  else printf(" %8d", uid);
  if (gid == (unsigned)-1) fputs(UID_NA, stdout);
  else printf(" %8d", gid);

  putchar(' ');
  fputs(dentries[dir], stdout);
  putchar('/');
  fputs(filename, stdout);
  fputs(endline, stdout);
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
  puts(srcfile);
  */
  c(dir, filename, uid, gid, mode);
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
  puts(target);
}

int opendir(const char* dir)
{
  if (last_dentry >= DENTRY_SLOTS) {
    fputs("instshow: error: Too many open directories!\n", stderr);
    exit(1);
  }
  dentries[last_dentry] = strdup(dir);
  return last_dentry++;
}

int opensubdir(int dir, const char* subdir)
{
  char* str;
  if (last_dentry >= DENTRY_SLOTS) {
    fputs("instshow: error: Too many open directories!\n", stderr);
    exit(1);
  }
  str = malloc(strlen(dentries[dir])+1+strlen(subdir)+1);
  strcpy(str, dentries[dir]);
  strcat(str, "/");
  strcat(str, subdir);
  dentries[last_dentry] = str;
  return last_dentry++;
}

int main(void)
{
  puts(" type/mode    owner    group path");
  insthier();
  return 0;
}
