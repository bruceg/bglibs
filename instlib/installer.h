/* $Id$ */
#ifndef SPAC__INSTALLER__H__
#define SPAC__INSTALLER__H__

/** \defgroup instlib instlib: Installation program generation

The installer libraries provide for building a program to install files,
to verify (check) an installation, and to show what would be installed,
from a single source.  This single source is typically called \c
insthier.c and contains a single routine also named \c insthier
(so-named as it provides the installation hierarchy).  The libraries
provide the \c main function that calls \c insthier as well as all the
necessary support functions.

Where given, \c uid and \c gid may be set to \c -1 if there is no need
to force the installed UID and GID to a particular value.  The \c dir
argument is a directory number previously opened by \c opendir.

@{ */

/** The directory prefix under which all files are placed. */
extern const char* install_prefix;

/** Open a directory and provide a file handle reference for future
 * operations. */
int opendir(const char* dir);

/** Open a subdirectory of a directory already opened by \c opendir. */
int opensubdir(int dir, const char* subdir);

/** Create a subdirectory. */
int d(int dir, const char* subdir,
      unsigned uid, unsigned gid, unsigned mode);

/** Copy a file from the source directory to a different name. */
void cf(int dir, const char* file, unsigned uid, unsigned gid, unsigned mode,
	const char* srcfile);

/** Copy a file from the source directory, retaining its name. */
void c(int dir, const char* file, unsigned uid, unsigned gid, unsigned mode);

/** Create a symbolic link. */
void s(int dir, const char* file, const char* target);

/** The single function that the \c insthier.c source needs to define. */
extern void insthier(void);

/** @} */

#endif
