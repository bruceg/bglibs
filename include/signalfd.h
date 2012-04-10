#ifndef UNIX__SIGNALFD__H__
#define UNIX__SIGNALFD__H__

/** \defgroup signalfd signalfd: Pass signals along a file.
@{ */

extern int signalfd_init(void);
extern void signalfd_catch(int sig);
extern void signalfd_uncatch(int sig);
extern void signalfd_close(void);

/** @} */

#endif
