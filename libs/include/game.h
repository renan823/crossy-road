#ifndef GAME_H_
#define GAME_H_

/* 
 * Define o contexto geral do jogo,
 * manipulado pelo servidor e
 * enviado ao cliente para renderizar o mapa.
 */
 
#include "map.h"
#include "object.h"

typedef struct game_context GameContext;
struct game_context {
	GameMap map;
	GameObject p1;
	GameObject p2;
	int sockets[2];
	int player_count;
	int winner;
};

GameContext *NewGameContext();
void DestroyGameContext(GameContext **ctx);

void UpdateEnemies(GameContext *ctx);
int AddPlayer(GameContext *ctx, int socket);
void RemovePlayer(GameContext *ctx, int client);
void ResetPlayer(GameContext *ctx, int socket);
void MovePlayer(GameContext *ctx, int socket, MoveDirection direction);

#endif