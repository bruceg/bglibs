#ifndef SPAC__INSTALLER__H__
#define SPAC__INSTALLER__H__

int opendir(const char* dir);
int opensubdir(int dir, const char* subdir);
int d(int dir, const char* subdir,
      unsigned uid, unsigned gid, unsigned mode);
void cf(int dir, const char* file, unsigned uid, unsigned gid, unsigned mode,
	const char* srcfile);
void c(int dir, const char* file, unsigned uid, unsigned gid, unsigned mode);
void s(int dir, const char* file, const char* target);

extern void insthier(void);

#endif
