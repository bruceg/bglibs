/* $Id$ */
#ifndef UNIX__CLOEXEC__H__
#define UNIX__CLOEXEC__H__

int cloexec_on(int fd);
int cloexec_off(int fd);

#endif
