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

    for (int i = 0; i < NUM_COLS; i++) {
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
            float y = rand() % (WINDOW_HEIGHT - ENEMY_HEIGHT);

            // Novo inimigo
            map->enemies[total].id = -1;
            map->enemies[total].x = x;
            map->enemies[total].y = y;
            map->enemies[total].width = ENEMY_WIDTH;
            map->enemies[total].height = ENEMY_HEIGHT;
            map->enemies[total].velocity = velocity;
            map->enemies[total].r = 1.0f;
            map->enemies[total].g = 0.0f;
            map->enemies[total].b = 0.0f;
            
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
    map->tiles[NUM_COLS - 1] = EndTile;

    // Controle
    int maxGrass = 2;
    int maxRoad = 4;

    int grassStreak = 1;
    int roadStreak = 0;

    // Alternar entre rua e grama
    for (int i = 1; i < NUM_COLS - 1; i++) {
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
GameMap *LoadMap() {
    GameMap *map = (GameMap *)malloc(sizeof(GameMap));
    if (map == NULL) {
        return NULL;
    }

    _LoadTerrain(map);
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

    free(*map);
    *map = NULL;
}
