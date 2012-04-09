/* bg-installer.c - Standard bglibs installer system.
 * Copyright (C) 2014  Bruce Guenter <bruce@untroubled.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */
#include "sysdeps.h"
#include <errno.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include "cli/cli.h"
#include "fmt/number.h"
#include "ibuf.h"
#include "obuf.h"
#include "iobuf.h"
#include "msg/msg.h"
#include "msg/wrap.h"
#include "path/path.h"
#include "str/str.h"
#include "str/iter.h"
#include "bg-installer-cli.h"

static const char* prefix;
static unsigned lineno = 0;
static str topdir;
static str path;
static str line;
static int errors = 0;

static int getpwnam_uid(const char* s, unsigned* i)
{
  struct passwd* pw;
  if ((pw = getpwnam(s)) != 0) {
    *i = pw->pw_uid;
    return 1;
  }
  return 0;
}

static int getgrnam_gid(const char* s, unsigned *i)
{
  struct group* gr;
  if ((gr = getgrnam(s)) != 0) {
    *i = gr->gr_gid;
    return 1;
  }
  return 0;
}

static int getint(const char* s, unsigned* i, int base, const char* desc,
		  int (*xlate)(const char*, unsigned*))
{
  char* end;
  if (*s == 0)
    *i = -1;
  else {
    *i = strtoul(s, &end, base);
    if (*end != 0) {
      if (xlate != 0) {
	if (!xlate(s, i)) {
	  warnf("{Line #}u{ has invalid }s{, skipping\n}", lineno, desc);
	  return 0;
	}
      }
      else {
	warnf("{Line #}u{ has invalid }s{ number, skipping\n}", lineno, desc);
	return 0;
      }
    }
  }
  return 1;
}

static void makepath(str* dest, const char* dir, const char* file)
{
  dest->len = 0;
  if (prefix != 0)
    wrap_str(str_copys(dest, prefix));
  wrap_str(str_join(dest, '/', &topdir));
  if (dir != 0)
    wrap_str(str_joins(dest, '/', dir));
  if (file != 0)
    wrap_str(str_joins(dest, '/', file));
}

static void pathsubst(str* dest, const str* template, const char* filename)
{
  striter i;
  int j;
  int k;
  const char* parts[10];
  static str partstr;

  /* Split up the filename on slashes */
  wrap_str(str_copys(&partstr, filename));
  str_subst(&partstr, '/', '\0');
  parts[0] = filename;
  for (j = 1, striter_start(&i, &partstr, '\0');
       j < 10 && striter_valid(&i);
       j++, striter_advance(&i))
    parts[j] = partstr.s + i.start;
  for (; j < 10; j++)
    parts[j] = 0;

  /* Substitute into global path */
  dest->len = 0;
  j = k = 0;
  while ((k = str_findnext(template, '$', j)) >= 0) {
    wrap_str(str_catb(dest, template->s+j, k-j));
    ++k;
    j = template->s[k];
    if (j >= '0' && j <= '9') {
      j -= '0';
      if (parts[j] != 0)
	wrap_str(str_cats(dest, parts[j]));
    }
    else if (j == '$')
      wrap_str(str_catc(dest, '$'));
    j = k + 1;
  }
  wrap_str(str_catb(dest, template->s+j, template->len-j));
}

static void error(const char* msg)
{
  errorf("{Path '}s{' }s", path.s, msg);
  ++errors;
}

static void show(char type, unsigned uid, unsigned gid, unsigned mode,
		 const char* a, const char* b)
{
  static const char UID_NA[] = "      N/A";

  if (!opt_verbose)
    return;
  
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

  if (uid == (unsigned)-1)
    obuf_puts(&outbuf, UID_NA);
  else
    obuf_putf(&outbuf, "\\ 8d", uid);
  if (gid == (unsigned)-1)
    obuf_puts(&outbuf, UID_NA);
  else
    obuf_putf(&outbuf, "\\ 8d", gid);

  obuf_putc(&outbuf, ' ');
  obuf_putstr(&outbuf, &path);
  if (a != 0)
    obuf_puts(&outbuf, a);
  if (b != 0)
    obuf_puts(&outbuf, b);
  obuf_endl(&outbuf);
}

static void setperms(const char* filename,
		     unsigned uid, unsigned gid, unsigned mode)
{
  if (chown(filename, uid, gid) != 0)
    diefsys(1, "{Could not set owner on '}s{'}", path.s);
  if (chmod(filename, mode) != 0)
    diefsys(1, "{Could not set permissions on '}s{'}", path.s);
}

static void checkmode(unsigned uid, unsigned gid,
		      unsigned mode, unsigned type)
{
  struct stat st;
  if (lstat(path.s, &st) == -1) {
    if (errno == ENOENT)
      error("does not exist");
    else
      diefsys(1, "{Could not stat '}s{'}", path.s);
  }
  else {
    if (uid != (unsigned)-1 && st.st_uid != uid)
      error("has wrong UID");
    if (gid != (unsigned)-1 && st.st_gid != gid)
      error("has wrong GID");
    if ((st.st_mode & 07777) != mode)
      error("has wrong permissions");
    if ((st.st_mode & ~07777) != type)
      error("is wrong file type");
  }
}

static void c(unsigned uid, unsigned gid, unsigned mode, const char* src)
{
  ibuf in;
  int out;
  static str pathtmp;

  show('-', uid, gid, mode, " <= ", src);

  if (!opt_dryrun) {
    if (unlink(path.s) != 0
	&& errno != ENOENT)
      diefsys(1, "{Could not remove '}s{'}", path.s);
    if (!ibuf_open(&in, src, 0))
      diefsys(1, "{Could not open '}s{'}", src);
    if ((out = path_mktemp(path.s, &pathtmp)) == -1)
      die1sys(1, "Could not create temporary file");
    if (!ibuf_copytofd(&in, out)
	|| close(out) != 0)
      diefsys(1, "{Could not write '}s{'}", pathtmp.s);
    setperms(pathtmp.s, uid, gid, mode);
    if (rename(pathtmp.s, path.s) != 0)
      diefsys(1, "{Could not rename '}s{' to '}s{'}", pathtmp.s, path.s);
    ibuf_close(&in);
  }

  if (opt_check) {
    checkmode(uid, gid, mode, S_IFREG);
  }
}

static void d(unsigned uid, unsigned gid, unsigned mode)
{
  struct stat st;
  show('d', uid, gid, mode, 0, 0);

  if (!opt_dryrun) {
    if (stat(path.s, &st) != 0) {
      if (path_mkdirs(path.s, 0700) == -1)
	diefsys(1, "{Could not create directory '}s{'}", path.s);
    }
    else if (!S_ISDIR(st.st_mode))
      dief(1, "{Path '}s{' exists but is not a directory}", path.s);
    setperms(path.s, uid, gid, mode);
  }

  if (opt_check) {
    checkmode(uid, gid, mode, S_IFDIR);
  }
}

static void l(unsigned uid, unsigned gid, unsigned mode, const char* src)
{
  int i;
  pid_t pid;
  const char* argv[] = {
    "libtool",
    "--silent",
    "--mode=install",
    "install",
    0, 0, /* -g GID */
    0, 0, /* -u UID */
    0, 0, /* -m MODE */
    0, /* srcfile */
    0, /* dstfile */
    0
  };
  char gidbuf[FMT_ULONG_LEN];
  char uidbuf[FMT_ULONG_LEN];
  char modebuf[FMT_ULONG_LEN];

  show('-', uid, gid, mode, 0, 0);

  if (!opt_dryrun) {
    i = 4;
    if (gid != (unsigned)-1) {
      gidbuf[fmt_udec(gidbuf, gid)] = 0;
      argv[i++] = "-g";
      argv[i++] = gidbuf;
    }
    if (uid != (unsigned)-1) {
      uidbuf[fmt_udec(uidbuf, uid)] = 0;
      argv[i++] = "-u";
      argv[i++] = uidbuf;
    }
    modebuf[fmt_unumw(modebuf, mode, 0, 0, 8, fmt_lcase_digits)] = 0;
    argv[i++] = "-m";
    argv[i++] = modebuf;
    argv[i++] = src;
    argv[i++] = path.s;

    if ((pid = fork()) == -1)
      die1sys(1, "Could not fork");
    if (pid == 0) {
      execvp(argv[0], (char**)argv);
      exit(111);
    }
    if (waitpid(pid, &i, 0) != pid)
      die1sys(1, "Failed to catch libtool exit");
    if (!WIFEXITED(i) || WEXITSTATUS(i) != 0)
      die1(1, "libtool failed");
  }

  if (opt_check) {
    checkmode(uid, gid, mode, S_IFREG);
  }
}

static void s(const char* src)
{
  char buf[4096];
  int len;

  if (src == 0) {
    warnf("{Line #}u{ is missing link source\n}", lineno);
    return;
  }

  show('l', -1, -1, 0777, " -> ", src);

  if (!opt_dryrun) {
    if (unlink(path.s) != 0
	&& errno != ENOENT)
      diefsys(1, "{Could not remove '}s{'}", path.s);
    if (symlink(src, path.s) != 0)
      diefsys(1, "{Could not create symlink '}s{'}", path.s);
  }

  if (opt_check) {
    if ((len = readlink(path.s, buf, sizeof buf)) == -1) {
      if (errno == ENOENT)
	error("does not exist");
      else
	diefsys(1, "{Could not read symlink '}s{'}", path.s);
    }
    else
      if (len != (int)strlen(src)
	  || memcmp(buf, src, len) != 0)
	error("contains wrong symlink");
  }
}

void setup_topdir(const char* newpath)
{
  if (newpath[0] != '/')
    die1(1, "Top directory must start with a slash");
  wrap_str(str_copys(&topdir, newpath));
  makepath(&path, 0, 0);
  if (!opt_dryrun) {
    if (path_mkdirs(path.s, 0777) != 0
	&& errno != EEXIST)
      diefsys(1, "{Could not create directory '}s{'}", topdir);
  }
}

void read_setup_topdir(const char* name)
{
  ibuf in;
  wrap_str(str_copy2s(&path, "conf-", name));
  if (!ibuf_open(&in, path.s, 0))
    diefsys(1, "{Could not open '}s{'}", path.s);
  if (!ibuf_getstr(&in, &line, LF))
    diefsys(1, "{Could not read line from '}s{'}", path.s);
  ibuf_close(&in);
  str_strip(&line);
  setup_topdir(line.s);
}

static void do_single(unsigned uid, unsigned gid, unsigned mode, const char* dir, const char* dest, const char* src)
{
  static str pathtmp;
  makepath(&pathtmp, dir, (dest == 0 || *dest == 0) ? src : dest);
  pathsubst(&path, &pathtmp, (src == 0) ? "" : src);

  if (line.s[1] == '?') {
    const char* testfile;
    struct stat st;
    testfile = (line.s[2] != 0) ? line.s + 2 : src;
    if (lstat(testfile, &st) == -1) {
      if (errno == ENOENT)
	return;
      else
	diefsys(1, "{Could not stat '}s{'}", testfile);
    }
  }

  switch (line.s[0]) {
  case 'c': c(uid, gid, mode, src); break;
  case 'd': d(uid, gid, mode); break;
  case 'l': l(uid, gid, mode, src); break;
  case 's': s(src); break;
  default:
    warnf("{Line #}u{ has invalid command letter, skipping\n}", lineno);
  }
}

static void do_action(unsigned uid, unsigned gid, unsigned mode, const char* dir, const char* dest, const char* src)
{
  static str matches;
  striter i;

  if (src == 0 || *src == 0)
    src = dest;

  if (src != 0 && has_magic(src)) {
    if (path_match(src, &matches, 0) < 0)
      diefsys(1, "{Could not path match '}s{'}", path.s);
    striter_loop(&i, &matches, '\0')
      do_single(uid, gid, mode, dir, dest, i.startptr);
  }
  else
    do_single(uid, gid, mode, dir, dest, src);
}

static void parse_line(void)
{
  const char* uidstr = 0;
  const char* gidstr = 0;
  const char* modestr = 0;
  const char* dir = 0;
  const char* file = 0;
  const char* src = 0;
  unsigned uid;
  unsigned gid;
  unsigned mode;
  int i;

  ++lineno;
  str_rstrip(&line);
  if (line.len == 0 || line.s[0] == '#')
    return;
  if (line.s[0] == '>') {
    read_setup_topdir(line.s+1);
    return;
  }

  str_subst(&line, ':', 0);
  if ((i = str_findfirst(&line, 0)) > 0) {
    uidstr = line.s + ++i;
    if ((i = str_findnext(&line, 0, i)) > 0) {
      gidstr = line.s + ++i;
      if ((i = str_findnext(&line, 0, i)) > 0) {
	modestr = line.s + ++i;
	if ((i = str_findnext(&line, 0, i)) > 0) {
	  dir = line.s + ++i;
	  if ((i = str_findnext(&line, 0, i)) > 0) {
	    file = line.s + ++i;
	    if ((i = str_findnext(&line, 0, i)) > 0)
	      src = line.s + ++i;
	  }
	}
      }
    }
  }

  if (dir == 0
      || (line.s[0] != 'd' && file == 0)) {
    warnf("{Line #}u{ is missing required fields\n}", lineno);
    return;
  }
  if (!getint(uidstr, &uid, 10, "UID", getpwnam_uid))
    return;
  if (!getint(gidstr, &gid, 10, "GID", getgrnam_gid))
    return;
  if (!getint(modestr, &mode, 8, "permissions", 0))
    return;

  do_action(uid, gid, mode, dir, file, src);
}

int cli_main(int argc, char* argv[])
{
  if (opt_check)
    opt_dryrun = 1;

  prefix = getenv("install_prefix");
  if (argv[0] != 0)
    setup_topdir(argv[0]);

  while (ibuf_getstr(&inbuf, &line, LF))
    parse_line();

  if (errors > 0)
    dief(1, "d{ errors were detected}", errors);
  return 0;
  (void)argc;
}
