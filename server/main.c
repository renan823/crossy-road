#include "server.h"
#include <stdlib.h>

/*
 * Função para iniciar o servidor.
 * Inclua as funções para receber requests.
 * Também lida com eventos que ocorrem
 * mesmo sem interação do player.
 */
int main(int argc, char *argv[]) {
	Server *server = StartServer(3000);
	if (server == NULL) {
		exit(1);
	}

	ServerListen(server);

	return 0;
}
