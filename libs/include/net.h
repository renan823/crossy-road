#ifndef NET_H_
#define NET_H_

#include <sys/socket.h>

/*
 * Definições dos tipos e funções
 * para lidar com sockets.
 */

typedef struct sockaddr_in SocketAddress;

int NewSocket(void);
int ConnectSocket(int socket, int port);

SocketAddress *NewSocketAdress(int port);
char *GetSocketAddressInfo(SocketAddress *address);

#endif