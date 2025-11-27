#include <arpa/inet.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../libs/include/semaphores.h"
#include "../libs/include/threads.h"
#include "../libs/include/game.h"
#include "../libs/include/net.h"

#include "server.h"

#define MAX_CONS 2

/*
 * 
 */
struct server {
    int socket;
    int port;
    SocketAddress *address;
    GameContext *ctx;
};

typedef struct handler_context HandlerContext;
struct handler_context {
	int client;
	GameContext *ctx;
};

/*
 * Lida com as requisições do cliente.
 */
void *_Handler(void *data, Semaphore *semaphore) {
	HandlerContext *ctx = (HandlerContext*) data;
	
	
	return NULL;
}

/*
 * Cria um novo "Server" com a porta
 * especificada.
 */
Server *StartServer(int port) {
 	Server *server = (Server *)malloc(sizeof(Server));
    if (server == NULL) {
        return NULL;
    }

    // Criar socket
    server->socket = NewSocket();
    if (server->socket == 0) {
        return NULL;
    }
    
    // Porta e endereço
    server->address = NewSocketAdress(port);
    server->port = port;
    
    // Inica os dados do jogo
    server->ctx = NewGameContext();
    
    return server;
}

/*
 * Fecha o socket e
 * desaloca o "Server".
 */
void StopServer(Server **server) {
    if (*server == NULL) {
        return;
    }
    
    // Apagar ctx
    DestroyGameContext(&(*server)->ctx);

    // Encerrar o socket
    close((*server)->socket);

    free(*server);
    *server = NULL;

    printf("Server stopped\n");
}

/*
 * 
 */
void ServerListen(Server *server) {
	if (server == NULL || server->ctx == NULL) {
        return;
    }

    // Bind
    if (bind(server->socket, (struct sockaddr *)server->address,
             sizeof(*server->address)) < 0) {
        return;
    }

    // Ativando servidor
    if (listen(server->socket, MAX_CONS) < 0) {
        return;
    }
    
    // Endereço para o cliente conectar
    SocketAddress address;
    socklen_t addrlen = sizeof(address);

    // Semáforo para usar junto das threads.
    Semaphore *semaphore = NewSemaphore();
    
    // Dados do handler
    HandlerContext handlerCtx = {
    	.ctx = server->ctx,
     	.client = 0
    };
    
    while (true) {
        // Atualizar inimigos
        UpdateEnemies(server->ctx);
        
        // Enviar atualização dos dados aos cliente
        
        // Nova conexão
        int client_socket =
            accept(server->socket, (struct sockaddr *)&address, &addrlen);
        if (client_socket < 0) {
            continue;
        }

        // Thread para lidar com cliente
        Thread *worker = NewThread(_Handler, &handlerCtx, semaphore);
        if (worker == NULL) {
            continue;
        }

        // Executa a thread
        RunThread(worker);

        // Encerra a conexão após atender
        DestroyThread(&worker);
    }

    DestroySemaphore(&semaphore);
}