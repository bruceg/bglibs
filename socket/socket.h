#ifndef SOCKET__H__
#define SOCKET__H__

typedef unsigned char ipv4addr[4];
extern ipv4addr IPV4ADDR_ANY;
extern ipv4addr IPV4ADDR_BROADCAST;

/* IP socket functions */
int socket_tcp(void);
int socket_udp(void);
int socket_connect4(int sock, const ipv4addr ip, unsigned short port);
int socket_bind4(int sock, const ipv4addr ip, unsigned short port);
int socket_accept4(int sock, ipv4addr ip, unsigned short* port);
int socket_recv4(int sock, char* buffer, unsigned buflen,
		 ipv4addr ip, unsigned short* port);
int socket_send4(int sock, const char* buffer, unsigned buflen,
		 const ipv4addr ip, unsigned short port);
int socket_getaddr4(int sock, ipv4addr ip, unsigned short* port);

/* UNIX local-domain socket functions */
int socket_unixdgm(void);
int socket_unixstr(void);
int socket_connectu(int sock, const char* path);
int socket_bindu(int sock, const char* path);
int socket_acceptu(int sock);
int socket_recvu(int sock, char* buffer, unsigned buflen);
int socket_sendu(int sock, const char* buffer, unsigned buflen);

/* Functions for any type of socket */
int socket_broadcast(int sock);
int socket_listen(int sock, int backlog);
int socket_reuse(int sock);
int socket_shutdown(int sock, int shut_rd, int shut_wr);

#endif
