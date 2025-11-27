#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "net.h"

/* 
 * Cria um novo socket e
 * inicializa sua configuração.
 * Retorna 0 em caso de erro.
 * Retorna o socket criado
 * em caso de sucesso.
 */
int NewSocket(void) {
	int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == 0) {
        return 0;
    }

    // Opção do socket
    int opt = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
       return 0;
    }
    
    return sock;
}

/*
 * Conecta um scoket cliente ao servidor.
 */
int ConnectSocket(int socket, int port) {
	// Endereço do servidor
	SocketAddress *addr = NewSocketAdress(port);
	
	if (inet_pton(AF_INET, "127.0.0.1", &addr->sin_addr) < 0) {
		printf("Invalid server address\n");
		return -1;
	}
	
	// Estabelece conexão
	if (connect(socket, (struct sockaddr*)addr, sizeof(*addr)) < 0) {
		printf("Connection failed\n");
		return -1;
	}
	
	printf("Connected!\n");
	
	return 0;
}

/*
 * Cria um novo "SocketAddress", inicializando
 * com dados da conexão: porta e tipo de endereço.
 */
SocketAddress *NewSocketAdress(int port) {
	SocketAddress *address = (SocketAddress*) malloc(sizeof(SocketAddress));
	if (address == NULL) {
		return NULL;
	}

    // Família IPv4
    address->sin_family = AF_INET;

    // Aceita conexões em qqr interface
    address->sin_addr.s_addr = INADDR_ANY;

    // Porta
    address->sin_port = htons(port);

    return(address); 
}

/*
 * Retorna, de forma formata, a string
 * correspondente ao endereço de IP e 
 * porta de um "SocketAddress" informado.
 */
char *GetSocketAddressInfo(SocketAddress *address) {
	if (address == NULL) {
		return NULL;
	}
	
	// Dados do ip
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &address->sin_addr, client_ip, INET_ADDRSTRLEN);
    
    // Dados da porta
    int client_port = ntohs(address->sin_port);
    
    // Retornar na string
    int info_size = INET_ADDRSTRLEN + 10;
    
    char *info = malloc(sizeof(char) * info_size);
    if (info == NULL) {
    	return NULL;
    }

    sprintf(info, "%s:%d", client_ip, client_port);
    
    return info;
}