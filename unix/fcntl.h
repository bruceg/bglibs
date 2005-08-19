/* $Id$ */
#ifndef UNIX__FCNTL__H__
#define UNIX__FCNTL__H__

int fcntl_fd_on(int fd, int flag);
int fcntl_fd_off(int fd, int flag);
int fcntl_fl_on(int fd, int flag);
int fcntl_fl_off(int fd, int flag);

#endif
