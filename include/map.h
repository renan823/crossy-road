#ifndef MAP_H_
#define MAP_H_

#include "object.h"

/*
 * Definições dos tipos e funções
 * para manipular mapas do jogo.
 */

#define MAP_WIDTH 20
#define MAP_HEIGTH 30
#define MAP_TILE_SIZE 2
#define MAP_NAME_SIZE 50

/*
 * Define o tipo de "chão" que player está.
 *
 * "River" é o único chão que
 * não poder ser pisado -> game over.
 */
typedef enum map_tile GameMapTile;
enum map_tile { Grass, Road, River, End };

/*
 * Define o tipo "GameMap".
 *
 * O mapa armazena um nome (não precisava ter
 * um nome, mas fica mais legal) e também
 * os items (tiles) dentro do mapa.
 *
 * Como os obstáculos e caminhos são
 * retângulos que ocupam a linha toda,
 * só precisamos salvar a informação
 * sobre 1 "pedaço" de cada linha, já que será
 * igual para todas as partes dela.
 *
 * Renderizar o mapa é ler o arquivo, colocar
 * em RAM e depois criar as linhas pra cada elemento
 * no vetor "tiles".
 */
typedef struct game_map GameMap;
struct game_map {
    char name[MAP_NAME_SIZE];
    GameMapTile tiles[MAP_HEIGTH];
    int enemies_count;
    GameObject **enemies;
};

GameMap *LoadMap(char *name);
void DestroyMap(GameMap **map);

#endif
