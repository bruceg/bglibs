#ifndef NET__SOCKET__H__
#define NET__SOCKET__H__

#include "ipv4.h"

/** \defgroup socket socket: General socket manipulations

\par Calling Convention

When calling a socket function, the socket file descriptor (where
required) will always be the first parameter.  Functions that return a
file descriptor (either creating a new socket or accepting a connection
on a socket return that file descriptor on success or \c -1 on error.
All other functions return non-zero (true) on success or \c 0 (false) on
error.  All functions set errno on error.

@{ */

/** \name IP socket functions
@{ */
int socket_tcp4(void);
int socket_tcp6(void);
#define socket_tcp socket_tcp4
int socket_udp4(void);
int socket_udp6(void);
#define socket_udp socket_udp4
int socket_connect4(int sock, const ipv4addr* ip, ipv4port port);
int socket_bind4(int sock, const ipv4addr* ip, ipv4port port);
int socket_accept4(int sock, ipv4addr* ip, ipv4port* port);
int socket_recv4(int sock, char* buffer, unsigned buflen,
		 ipv4addr* ip, ipv4port* port);
int socket_send4(int sock, const char* buffer, unsigned buflen,
		 const ipv4addr* ip, ipv4port port);
int socket_getaddr4(int sock, ipv4addr* ip, ipv4port* port);
int socket_cork(int sock);
int socket_uncork(int sock);
int socket_notcpdelay(int sock, int enable);
/* @} */

/** \name UNIX local-domain socket functions
@{ */
int socket_unixdgm(void);
int socket_unixstr(void);
int socket_connectu(int sock, const char* path);
int socket_bindu(int sock, const char* path);
int socket_acceptu(int sock);
int socket_recvu(int sock, char* buffer, unsigned buflen);
int socket_sendu(int sock, const char* buffer, unsigned buflen);

int socket_pairstr(int fd[2]);
int socket_pairdgm(int fd[2]);
/* @} */

/** \name Functions for any type of socket
@{ */
int socket_broadcast(int sock);
int socket_connected(int sock);
int socket_linger(int fd, int onoff, int seconds);
int socket_listen(int sock, int backlog);
int socket_reuse(int sock);
int socket_shutdown(int sock, int shut_rd, int shut_wr);

int socket_sendfd(int sock, int fd);
int socket_recvfd(int sock);
/* @} */

/* @} */

#endif
