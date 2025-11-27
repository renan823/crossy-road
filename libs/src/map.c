#include <stdlib.h>
#include <time.h>

#include "map.h"
#include "object.h"

/*
 * Gera os inimigos.
 * Atribui a cor vermelha para os carros.
 * As posições verticais são aleatorias.
 * A velocidade pode ser positiva ou negativa.
 */
void _LoadEnemies(GameMap *map) {
    if (map == NULL) {
        return;
    }

    // Semente aleatória
    srand(time(NULL));

    int total = 0;

    for (int i = 0; i < map->size; i++) {
        // Ignora tiles que não são rua
        if (map->tiles[i] != RoadTile) {
            continue;
        }

        // Velocidade dos inimigos da faixa
        // Falta ajustar aqui!!!
        float velocity = 0.3f + (rand() % 8) * 0.3f;
        if (rand() % 2 == 0) {
            velocity = -velocity;
        } else {
            velocity = velocity;
        }

        // 2 inimigos por faixa
        for (int j = 0; j < 2; j++) {
            // Posições
            float x = (i * TILE_SIZE) + ((TILE_SIZE - ENEMY_WIDTH) / 2.0);
            float y = rand() % (map->height - ENEMY_HEIGHT);

            // Novo inimigo
            GameObject *enemy =
                NewGameObject(-1, x, y, ENEMY_WIDTH, ENEMY_HEIGHT, velocity,
                              1.0f, 0.0f, 0.0f);

            map->enemies[total] = enemy;
            total++;
        }
    }
}

/*
 * Gera o terreno.
 * Tenta fazer algo pseudo aleatório,
 * mas que não seja muit difícil de jogar.
 */
void _LoadTerrain(GameMap *map) {
    if (map == NULL) {
        return;
    }

    // Semente aleatória
    srand(time(NULL));

    // Primeria e ultima faixa sempre seguras
    map->tiles[0] = GrassTile;
    map->tiles[map->size - 1] = EndTile;

    // Controle
    int maxGrass = 2;
    int maxRoad = 4;

    int grassStreak = 1;
    int roadStreak = 0;

    // Alternar entre rua e grama
    for (int i = 1; i < map->size - 1; i++) {
        int r = rand() % 100; // 0–99

        // Muitas ruas -> vai pra grama
        if (roadStreak >= maxRoad) {
            roadStreak = 0;
            grassStreak++;
            map->tiles[i] = GrassTile;
            continue;
        }

        // Muita grama -> vai pra rua
        if (grassStreak >= maxGrass) {
            grassStreak = 0;
            roadStreak++;
            map->tiles[i] = RoadTile;
            continue;
        }

        // Senão, gera aleatorio msm
        // Usa 40% de grama pra 60% de ruas
        if (r < 40) {
            map->tiles[i] = GrassTile;
            grassStreak++;
            roadStreak = 0;
        } else { // 60% rua
            map->tiles[i] = RoadTile;
            roadStreak++;
            grassStreak = 0;
        }
    }
}

/*
 * Cria um novo mapa com terreno e inimigos.
 */
GameMap *LoadMap(int width, int height, int size) {
    GameMap *map = (GameMap *)malloc(sizeof(GameMap));
    if (map == NULL) {
        return NULL;
    }

    map->width = width;
    map->height = height;

    // Carrega terreno
    map->size = size;
    map->tiles = (GameMapTile *)malloc(sizeof(GameMapTile) * map->size);
    _LoadTerrain(map);

    // Carrega inimigos (2 x faixa)
    for (int i = 0; i < map->size; i++) {
        if (map->tiles[i] == RoadTile) {
            map->enemies_count += 2;
        }
    }

    map->enemies = (GameObject **)malloc(sizeof(GameObject *) * map->size);
    _LoadEnemies(map);

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
