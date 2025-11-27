#include <stdlib.h>

#include "map.h"
#include "object.h"

#include "game.h"

GameContext *NewGameContext() {
	GameContext *ctx = (GameContext*) malloc(sizeof(GameContext));
	if (ctx == NULL) {
		return NULL;
	}
	
	// Carregar dados
	ctx->map = LoadMap(WINDOW_WIDTH, WINDOW_HEIGHT, NUM_COLS);
	ctx->p1 = NULL;
	ctx->p2 = NULL;
	ctx->player_count = 0;
	
	return ctx;
}

void DestroyGameContext(GameContext **ctx) {
	
}

void UpdateEnemies(GameContext *ctx) {
	if (ctx == NULL) {
		return;
	}
	
	// Atualiza inimigos movendo-os
	for (int i = 0; i < ctx->map->enemies_count; i++) {
		GameObject *enemy = ctx->map->enemies[i];
		
		enemy->y += enemy->velocity;
		
		// Verificar se saiu da tela
        if (enemy->y + enemy->height < 0 || enemy->y > WINDOW_HEIGHT) {
      		if (enemy->velocity > 0) {
      		enemy->y = -enemy->height;
            } else {
                enemy->y = WINDOW_HEIGHT;
            }
        }
	}
}