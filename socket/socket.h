#ifndef SOCKET__H__
#define SOCKET__H__

typedef char ipv4addr[4];
#define IP_ANY ((ipv4addr)"\0\0\0\0")

int socket_tcp(void);
int socket_udp(void);
int socket_connect4(int sock, const ipv4addr ip, unsigned short port);
int socket_bind4(int sock, const ipv4addr ip, unsigned short port);
int socket_reuse(int sock);
int socket_listen(int sock, int backlog);
int socket_accept4(int sock, ipv4addr ip, unsigned short* port);
int socket_recv4(int sock, char* buffer, unsigned buflen,
		 ipv4addr ip, unsigned short* port);
int socket_send4(int sock, const char* buffer, unsigned buflen,
		 const ipv4addr ip, unsigned short port);

#endif
