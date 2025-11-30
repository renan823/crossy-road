#ifndef MAP_H_
#define MAP_H_

#include "object.h"

/*
 * Definições dos tipos e funções
 * para manipular mapas do jogo.
 */

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

// Faixas de terreno
#define TILE_SIZE 30
#define NUM_COLS (WINDOW_WIDTH / TILE_SIZE)

#define NUM_ENEMIES NUM_COLS * 2

#define PLAYER_SIZE 20

// Dimensões do Carro
#define CAR_WIDTH 22
#define CAR_HEIGHT 45

/*
 * Define o tipo de "chão" que player está.
 *
 * "River" é o único chão que
 * não poder ser pisado -> game over.
 */
typedef enum map_tile GameMapTile;
enum map_tile { GrassTile, RoadTile, EndTile };

/*
 * Define o tipo "GameMap".
 *
 * Como os obstáculos e caminhos são
 * retângulos que ocupam a linha toda,
 * só precisamos salvar a informação
 * sobre 1 "pedaço" de cada linha, já que será
 * igual para todas as partes dela.
 */
typedef struct game_map GameMap;
struct game_map {
    GameMapTile tiles[NUM_COLS];
    GameObject enemies[NUM_ENEMIES];
};

GameMap *LoadMap();
void DestroyMap(GameMap **map);

#endif
