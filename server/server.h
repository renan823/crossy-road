#ifndef SERVER_H_
#define SERVER_H_

#include <stdbool.h>

typedef struct server Server;

Server *StartServer(int port);
void StopServer(Server **server);
void ServerListen(Server *server);

#endif
