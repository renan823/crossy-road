#include <arpa/inet.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#include "net.h"
#include "semaphores.h"
#include "threads.h"

#include "server.h"

/*
 * Define a struct "Server".
 *
 * Armazena o socket UNIX para
 * conexão e criar o servidor
 * usando TCP.
 *
 * O "ServerHandler" é a função
 * que vai lidar com requisições
 * dos clientes.
 * É o user que define essa função.
 *
 * O "context" define variáveis de
 * ambiente que o usuário pode
 * querer usar e passar pro
 * "handler".
 */
struct server {
    int socket;
    int port;
    int cons;
    bool logging;
    SocketAddress *address;
    ServerHandler *handler;
    void *context;
};

/*
 * Define um pacote de dados que é
 * passado pela thread e usado pelo
 * handler do usuário.
 */
typedef struct handler_context HandlerContext;
struct handler_context {
    int client;
    void *data;
    ServerHandler *handler;
};

/*
 * Executa de fato a função especificada
 * pelo usuário para manipular uma
 * requisição feito ao servidor.
 *
 * O socket do cliente, dados (ambiente) e
 * um semáforo são enviados ao handler do usuário.
 *
 * Cabe ao usuário usar esses dados como for melhor.
 */
void *_HandleClient(void *handler_ctx, Semaphore *semaphore) {
    // Pega os dados como contexto
    HandlerContext *context = (HandlerContext *)handler_ctx;

    // Chama a função do usuário
    context->handler(context->client, context->data, semaphore);
    
    // Fecha o cliente
    close(context->client);

    return NULL;
}

/*
 * Cria um novo "Server" com a porta
 * especificada.
 * Os dados de contexto deve ser passados
 * como ponteiro "void*" - O user que
 * manda como vai lidar com isso :)
 */
Server *StartServer(int port, int cons, bool logging, void *context) {
    Server *server = (Server *)malloc(sizeof(Server));
    if (server == NULL) {
        return NULL;
    }

    // Criar socket
    server->socket = NewSocket();
    if (server->socket == 0) {
        return NULL;
    }

    // Contexto (dados) para serem usados
    server->context = context;

    // Porta e endereço
    server->address = NewSocketAdress(port);
    server->port = port;
    server->cons = cons;

    // Opções pra print de conexões
    server->logging = logging;

    // Por default, handler é NULL
    server->handler = NULL;

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

    // Encerrar o socket
    close((*server)->socket);

    free(*server);
    *server = NULL;

    printf("Server stopped\n");
}

/*
 * Define a função que vai
 * lidar com as conexões de clientes.
 * Pode ser qualquer função que siga
 * o padrão do "ServerHandler".
 * Os dados do "context" são passados
 * pra essa função especificada pelo
 * user, assim pode mainuplar dados
 * como quiser.
 *
 * O handler é executado numa thread separada.
 * No handler está incluso um "Semaphore".
 */
void ServerRegisterHandler(Server *server, ServerHandler *handler) {
    if (server == NULL) {
        return;
    }

    server->handler = handler;
}

/*
 * Inicia o servidor e faz
 * ele "ouvir" na porta especificada.
 */
void ServerListen(Server *server) {
    if (server == NULL || server->handler == NULL) {
        return;
    }

    // Bind
    if (bind(server->socket, (struct sockaddr *) server->address,
             sizeof(*server->address)) < 0) {
        return;
    }

    // Ativando servidor
    if (listen(server->socket, server->cons) < 0) {
        return;
    }

    if (server->logging) {
	   	char *info = GetSocketAddressInfo(server->address);
	    if (info != NULL) {
	        printf("[SERVER] Server running on %s\n", info);
	        free(info);
	    }
    }

    // Endereço para o cliente conectar
    SocketAddress address;
    socklen_t addrlen = sizeof(address);

    // Semáforo para usar junto das threads.
    Semaphore *semaphore = NewSemaphore();

    // Dados de contexto para o handler
    // Só muda o cliente
    HandlerContext handler_context = {
        .handler = server->handler, .data = server->context, .client = 0};

    // Loop para receber as conexões
    while (true) {

        // Nova conexão
        int client_socket =
            accept(server->socket, (struct sockaddr *)&address, &addrlen);
        if (client_socket < 0) {
            continue;
        }

        /*
         * Lida com a requisição do cliente
         * criando uma nova thread para isso.
         * Nova conexão = nova thread.
         *
         * No "ServerHandler" são passados dados
         * e tbm a parte de gerenciar o contexto
         * que inclui o semáforo (se necessário).
         */
        handler_context.client = client_socket;

        Thread *worker = NewThread(_HandleClient, &handler_context, semaphore);
        if (worker == NULL) {
            continue;
        }

        // Executa a thread
        RunThread(worker);
        
        // Se permitir print (logs), mostra a conexão
        if (server->logging) {
            char *info = GetSocketAddressInfo(&address);
            if (info != NULL) {
                printf("[SERVER] Connection from %s\n", info);
                free(info);
            }
        }
        
        // Encerra a conexão após atender
        DestroyThread(&worker);
    }

    DestroySemaphore(&semaphore);
}
