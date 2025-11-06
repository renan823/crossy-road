#ifndef NET_H_
#define NET_H_

#include <sys/socket.h>

typedef struct sockaddr_in SocketAddress;

int NewSocket(void);

SocketAddress *NewSocketAdress(int port);
char *GetSocketAddressInfo(SocketAddress *address);

#endif