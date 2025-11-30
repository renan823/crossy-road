#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

#include "../libs/include/net.h"
#include "../libs/include/threads.h"
#include "game.h"
#include "object.h"
#include "screen.h"
#include "semaphores.h"

#define SERVER_PORT 3000

// Variável global que representa esteplayer
int client;
int id;

// Contexto do jogo
static GameContext *ctx;

/*
 * Detecta teclas e envia mensagens ao
 * server para mover.
 */
void _OnKeyPress(unsigned char key, int x, int y) {
    GameEvent event;
    event.type = MoveEvent;

    switch (key) {
    case 'a':
        event.direction = MoveLeft;
        break;
    case 'w':
        event.direction = MoveUp;
        break;
    case 's':
        event.direction = MoveDown;
        break;
    case 'd':
        event.direction = MoveRight;
        break;
    case 27: // ESC
        exit(0);
    }

    send(client, &event, sizeof(event), 0);
}

/*
 * Função principal do loop
 */
void _GameLoop(void) {
    ClearScreen();

    GameContext *game = ctx;

    if (game->player_count < 2) {
        DrawLoading();
    } else if (game->winner != -1) {
        if (game->winner == id) {
            DrawWin();
        } else {
            DrawLose();
        }
    } else {
        DrawMap(&game->map);
        DrawEnemies(&game->map);
        DrawPlayers(game);

        UpdateEnemies(game);
    }

    FlushScreen();
}

/*
 * Thread separada para lidar com as
 * mensagens do servidor,
 * sem bloquear o opnegl.
 */
void *_GameWorker(void *data, Semaphore *semaphore) {
    GameContext state;

    while (true) {
        if (recv(client, &state, sizeof(state), 0) < 0) {
            exit(1);
        }

        SemaphoreLock(semaphore);
        *ctx = state;
        SemaphoreUnlock(semaphore);
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    // Connectar ao servidor
    client = NewSocket();
    if (ConnectSocket(client, SERVER_PORT) < 0) {
        exit(1);
    }

    // Enviar mensagem de conexão ao server
    GameEvent conn = {.type = ConnectEvent};
    if (send(client, &conn, sizeof(conn), 0) != sizeof(conn)) {
        exit(1);
    }
    
    // Receber id do jogo
    if (recv(client, &id, sizeof(int), 0) <= 0 ) {
         printf("Falha em receber o ID\n");
         exit(1);
    }
    
    printf("Meu ID: %d\n", id);

    ctx = NewGameContext();

    // Thread separada para comunicar
    Semaphore *semaphore = NewSemaphore();
    Thread *worker = NewThread(_GameWorker, NULL, semaphore);
    RunThread(worker);

    // Inicia a tela opengl
    InitScreen(argc, argv, _GameLoop);
    SetScreenKeybinds(_OnKeyPress);
    ScreenLoop();

    return 0;
}
