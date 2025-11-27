#include <stdlib.h>

#include "../libs/include/net.h"
#include "game.h"
#include "object.h"
#include "screen.h"

#define SERVER_PORT 3000

// Variável global que representa esteplayer
int client;

// Contexto do jogo
static GameContext *ctx;

/*
 * Função principal do loop
 */
void _GameLoop(void) {
    ClearScreen();

    GameContext *game = ctx;

    DrawMap(game->map);
    DrawEnemies(game->map);
    DrawPlayers(game);
    
    UpdateEnemies(game);

    FlushScreen();
}

int main(int argc, char *argv[]) {
    /*Connectar ao servidor
    client = NewSocket();
    if (ConnectSocket(client, SERVER_PORT) < 0) {
            exit(1);
    }*/

    // Contexto do jogo
    ctx = NewGameContext();

    // Players
    ctx->p1 = NewGameObject(1, (TILE_SIZE - PLAYER_SIZE) / 2.0,
                            (WINDOW_HEIGHT / 2.0) + 20, PLAYER_SIZE,
                            PLAYER_SIZE, 0, 1.0f, 1.0f, 1.0f);
    ctx->p2 = NewGameObject(2, (TILE_SIZE - PLAYER_SIZE) / 2.0,
                            (WINDOW_HEIGHT / 2.0) - 40, PLAYER_SIZE,
                            PLAYER_SIZE, 0, 0.0f, 0.0f, 0.0f);

    // Inicia a tela opengl
    InitScreen(argc, argv, _GameLoop);
    ScreenLoop();

    return 0;
}
