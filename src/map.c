#include <stdio.h>
#include <stdlib.h>

#include "map.h"

/*
 * Lê um arquivo ".map" e carrega os dados
 * do mapa atual.
 */
GameMap *LoadMap(char *name) {
	GameMap *map = (GameMap*) malloc(sizeof(GameMap));
	if (map == NULL) {
		return NULL;
	}
	
	// Carregar arquivo do .map
	FILE *file = fopen(name, "rb");
	if (file == NULL) {
		return NULL;
	}
	
	// Ler dados do map
	if (fread(map->name, sizeof(char), MAP_NAME_SIZE, file) <= 0) {
		fclose(file);
		return NULL;
	}
	
	if (fread(map->tiles, sizeof(GameMapTile), MAP_HEIGTH, file) <= 0) {
		fclose(file);
		return NULL;
	
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
	
	free(*map);
	*map = NULL;
}