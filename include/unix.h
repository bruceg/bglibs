#ifndef BGLIBS_UNIX__H__
#define BGLIBS_UNIX__H__

int cloexec_on(int fd);
int cloexec_off(int fd);

int fcntl_fd_on(int fd, int flag);
int fcntl_fd_off(int fd, int flag);
int fcntl_fl_on(int fd, int flag);
int fcntl_fl_off(int fd, int flag);

int nonblock_on(int fd);
int nonblock_off(int fd);

#endif
