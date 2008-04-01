#ifndef BGLIBS__NET__CMSG__H__
#define BGLIBS__NET__CMSG__H__

#ifdef SOLARIS
#define _XOPEN_SOURCE 500
#endif

#include <sys/socket.h>

#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL 0
#endif

#ifndef CMSG_ALIGN
#define CMSG_ALIGN(len) (((len) + sizeof (size_t) - 1) \
                         & (size_t) ~(sizeof (size_t) - 1))
#endif

#ifndef CMSG_SPACE
#define CMSG_SPACE(len) (CMSG_ALIGN(sizeof(struct cmsghdr)) + CMSG_ALIGN(len))
#endif

#ifndef CMSG_LEN
#define CMSG_LEN(len) (CMSG_ALIGN(sizeof(struct cmsghdr)) + (len))
#endif

#endif
