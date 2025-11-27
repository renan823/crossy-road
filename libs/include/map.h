#ifndef MAP_H_
#define MAP_H_

#include "object.h"

/*
 * Definições dos tipos e funções
 * para manipular mapas do jogo.
 */

 #define TILE_SIZE 30 

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
	int width;
    int height;
    int size;
    GameMapTile *tiles;
    int enemies_count;
    GameObject **enemies;
};

GameMap *LoadMap(int width, int height, int size);
void DestroyMap(GameMap **map);

#endif
