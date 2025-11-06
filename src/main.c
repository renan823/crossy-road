#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>

#include "semaphores.h"
#include "server.h"

#define BUFFER_SIZE 1024

/*
 * Exemplo de handler
 */
void _TesteHandler(int client, Semaphore *semaphore, void *context) {
    char buffer[BUFFER_SIZE] = {0};

    ssize_t valread = recv(client, buffer, BUFFER_SIZE, 0);
    if (valread < 0) {
        return;
    }
    
    // Acão que trava
    SemaphoreLock(semaphore);
    // .....
    SemaphoreUnlock(semaphore);

    char *response = "Generic response!\n";
    send(client, response, strlen(response), 0);
}

int main(void) {
	Server *server = StartServer(3000, 3, true, NULL);
	if (server == NULL) {
		exit(1);
	}
	
	ServerRegisterHandler(server, &_TesteHandler);
	
	ServerListen(server);
	StopServer(&server);
	
	return 0;
}