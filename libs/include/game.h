#ifndef GAME_H_
#define GAME_H_

/* 
 * Define o contexto geral do jogo,
 * manipulado pelo servidor e
 * enviado ao cliente para renderizar o mapa.
 */
 
#include "map.h"
#include "object.h"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

// Faixas de terreno 
#define NUM_COLS (WINDOW_WIDTH / TILE_SIZE)

#define PLAYER_SIZE 20 

// Dimensões do Carro
#define CAR_WIDTH 22
#define CAR_HEIGHT 45

typedef struct game_context GameContext;
struct game_context {
	GameMap *map;
	GameObject *p1;
	GameObject *p2;
	int sockets[2];
	int player_count;
};

GameContext *NewGameContext();
void DestroyGameContext(GameContext **ctx);

void UpdateEnemies(GameContext *ctx);

#endif