// pra usar nanosleep
#define _POSIX_C_SOURCE 199309L

#include <arpa/inet.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>

#include "../libs/include/game.h"
#include "../libs/include/net.h"
#include "../libs/include/semaphores.h"
#include "../libs/include/threads.h"
#include "object.h"

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
 * Para executar mais "liso"
 */
void _Sleep(long ms) {
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

/*
 * Lida com as requisições do cliente.
 */
void *_Handler(void *data, Semaphore *semaphore) {
    HandlerContext *ctx = (HandlerContext *)data;
    GameEvent event;

    while (1) {
        ssize_t n = recv(ctx->client, &event, sizeof(event), 0);

        // Cliente desconectou
        if (n == 0) {
            printf("Player %d desconectou\n", ctx->ctx->sockets[0] == ctx->client ? ctx->ctx->p1.id : ctx->ctx->p2.id);
            
            // Remover client
            SemaphoreLock(semaphore);
            RemovePlayer(ctx->ctx, ctx->client);
            SemaphoreUnlock(semaphore);
            
            close(ctx->client);
            free(ctx);
            return NULL;
        }

        // Erro
        if (n < 0) {
        	printf("Player %d desconectou\n", ctx->ctx->sockets[0] == ctx->client ? ctx->ctx->p1.id : ctx->ctx->p2.id);
         
            // Remover client
            SemaphoreLock(semaphore);
            RemovePlayer(ctx->ctx, ctx->client);
            SemaphoreUnlock(semaphore);
            
            close(ctx->client);
            free(ctx);
            return NULL;
        }

        SemaphoreLock(semaphore);

        switch (event.type) {
        case ConnectEvent:
            if (ctx->ctx->player_count < MAX_CONS) {
                int id = AddPlayer(ctx->ctx, ctx->client);
                printf("Player %d conectado!\n", id);
                // Dados do player
                send(ctx->client, &id, sizeof(int), 0);
                
                // Dados do mapa
                send(ctx->client, ctx->ctx, sizeof(*ctx->ctx), 0);
            }
            break;

        case MoveEvent:
            MovePlayer(ctx->ctx, ctx->client, event.direction);
            break;

        default:
            break;
        }

        SemaphoreUnlock(semaphore);
    }

    return NULL;
}

/*
 * Thread separada para dados do jogo.
 * Fica inativa em certos tempos
 * pra deixar mais fluido (tipo FPS)
 */
void *_GameWorker(void *data, Semaphore *semaphore) {
    Server *server = (Server *)data;
    GameContext *ctx = server->ctx;

    while (1) {
    	_Sleep(16); // Tipo 60 fps

        if (ctx->player_count == 2) {

            SemaphoreLock(semaphore);
            UpdateEnemies(ctx);
            SemaphoreUnlock(semaphore);

            for (int i = 0; i < ctx->player_count; i++) {
                int sock = ctx->sockets[i];
                send(sock, ctx, sizeof(*ctx), 0);
            }
        }
    }

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

    printf("Servidor parado\n");
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

    printf("Servidor iniciado\n");

    // Semáforo para usar junto das threads.
    Semaphore *semaphore = NewSemaphore();

    // Thread para atualizar dados do jogo
    Thread *gameThread = NewThread(_GameWorker, server, semaphore);
    RunThread(gameThread);

    while (true) {
        // Endereço para o cliente conectar
        SocketAddress address;
        socklen_t addrlen = sizeof(address);

        // Nova conexão
        int client_socket =
            accept(server->socket, (struct sockaddr *)&address, &addrlen);
        if (client_socket < 0) {
            continue;
        }

        // Context da thread
        HandlerContext *ctx = malloc(sizeof(HandlerContext));
        ctx->ctx = server->ctx;
        ctx->client = client_socket;

        // Cria thread para esse cliente
        Thread *worker = NewThread(_Handler, ctx, semaphore);
        RunThread(worker);
    }

    DestroySemaphore(&semaphore);
}
