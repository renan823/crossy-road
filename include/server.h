#ifndef SERVER_H_
#define SERVER_H_

#include <stdbool.h>

#include "semaphores.h"

/*
 * Definições dos tipos e funções
 * para a biblioteca de servidores TCP.
 */

typedef void ServerHandler(int, Semaphore*, void*);

typedef struct server Server;

Server *StartServer(int port, int cons, bool logging, void *context);
void StopServer(Server **server);

void ServerRegisterHandler(Server *server, ServerHandler *handler);
void ServerListen(Server *server);

#endif
