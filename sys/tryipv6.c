#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(void) {
  struct sockaddr_in6 sa;
  sa.sin6_family = AF_INET6;
  return 0;
}
