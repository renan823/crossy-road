#include <stdio.h>
#include <stdlib.h>

#include "map.h"
#include "object.h"

/*
 * Carrega um inimigo do arquivo de mapa
 * para a memória.
 */
void _LoadEnemy(GameMap *map, FILE *file, int i) {
    if (map == NULL || file == NULL) {
        return;
    }

    GameObject *enemy = (GameObject *)malloc(sizeof(GameObject));
    if (enemy == NULL) {
        return;
    }

    // Ler atributos do inimigo
    fread(&enemy->id, sizeof(int), 1, file);
    fread(&enemy->pos_x, sizeof(int), 1, file);
    fread(&enemy->pos_y, sizeof(int), 1, file);
    fread(&enemy->width, sizeof(int), 1, file);
    fread(&enemy->height, sizeof(int), 1, file);
    fread(&enemy->velocity, sizeof(double), 1, file);
    fread(enemy->color, sizeof(char), 8, file);

    map->enemies[i] = enemy;

    return;
}

/*
 * Lê um arquivo ".map" e carrega os dados
 * do mapa atual.
 */
GameMap *LoadMap(char *name) {
    GameMap *map = (GameMap *)malloc(sizeof(GameMap));
    if (map == NULL) {
        return NULL;
    }

    // Carregar arquivo do .map
    FILE *file = fopen(name, "rb");
    if (file == NULL) {
        return NULL;
    }

    // Ler dados do map

    // Ler nome do mapa
    if (fread(map->name, sizeof(char), MAP_NAME_SIZE, file) <= 0) {
        fclose(file);
        return NULL;
    }

    // Ler os tiles
    if (fread(map->tiles, sizeof(GameMapTile), MAP_HEIGTH, file) <= 0) {
        fclose(file);
        return NULL;
    }

    // Ler numero de inimigos
    if (fread(&map->enemies_count, sizeof(int), 1, file)) {
        fclose(file);
        return NULL;
    }

    // Alocação do vetor de inimigos
    map->enemies = (GameObject **)malloc(sizeof(GameObject *));
    if (map->enemies == NULL) {
        fclose(file);
        return NULL;
    }

    // Ler inimigos
    for (int i = 0; i < map->enemies_count; i++) {
        _LoadEnemy(map, file, i);
    }

    fclose(file);

    return map;
}

/*
 * Desaloca a struct "GameMap".
 */
void DestroyMap(GameMap **map) {
    if (*map == NULL) {
        return;
    }
    
    // Desaloca os inimigos
    if ((*map)->enemies != NULL) {
        for (int i = 0; i < (*map)->enemies_count; i++) {
            DestroyGameObject(&(*map)->enemies[i]);
        }

        free((*map)->enemies);
    }

    free(*map);
    *map = NULL;
}
