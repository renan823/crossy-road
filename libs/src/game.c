#include <stdlib.h>
#include <sys/random.h>

#include "map.h"
#include "object.h"

#include "game.h"

/*
 * Inicializa um novo game context
 */
GameContext *NewGameContext() {
    GameContext *ctx = (GameContext *)malloc(sizeof(GameContext));
    if (ctx == NULL) {
        return NULL;
    }

    // Carregar dados
    ctx->map = *LoadMap();
    ctx->p1.id = -1;
    ctx->p2.id = -1;
    ctx->player_count = 0;
    ctx->winner = -1;

    return ctx;
}

void DestroyGameContext(GameContext **ctx) {}

/*
 * Atualiza a posição dos inimigos.
 */
void UpdateEnemies(GameContext *ctx) {
    if (ctx == NULL) {
        return;
    }

    // Atualiza inimigos movendo-os
    for (int i = 0; i < NUM_ENEMIES; i++) {
        GameObject *enemy = &ctx->map.enemies[i];

        enemy->y += enemy->velocity;
        
        // Verificar colisão carro player
        if (Collision(enemy, &ctx->p1)) {
       		ResetPlayer(ctx, ctx->sockets[0]);
        }
        
        if (Collision(enemy, &ctx->p2)) {
       		ResetPlayer(ctx, ctx->sockets[1]);
        }

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

/*
 * Id para o player;
 */
int _NewID() {
    int value;
    if (getrandom(&value, sizeof(value), 0) != sizeof(value)) {
        value = rand();
    }
    
    if (value < 0) {
    	value *= -1;
    }

    return value;
}

/*
 * Preenche dados do player.
 */
int _NewPlayer(GameObject *p, int count) {
    p->id = _NewID();
    p->width = PLAYER_SIZE;
    p->height = PLAYER_SIZE;
    p->velocity = 0;

    // Cor
    p->r = (float)count;
    p->g = (float)count;
    p->b = (float)count;

    p->x = (TILE_SIZE - PLAYER_SIZE) / 2.0;
    
    return p->id;
}

/*
 * Adiciona um novo player ao jogo.
 */
int AddPlayer(GameContext *ctx, int socket) {
    if (ctx == NULL || ctx->player_count >= 2) {
        return -1;
    }
    
    int id;

    if (ctx->player_count == 0) {
        id = _NewPlayer(&ctx->p1, 0);
        ctx->sockets[0] = socket;

        // Ajuste das posições
        ctx->p1.y = (WINDOW_HEIGHT / 2.0) + 20;
    } else {
        id = _NewPlayer(&ctx->p2, 1);
        ctx->sockets[1] = socket;

        // Ajuste das posições
        ctx->p2.y = (WINDOW_HEIGHT / 2.0) - 40;
    }

    ctx->player_count++;
    
    return id;
}

/*
 * Reinicia a posição de um player
 * após morrer.
 */
void ResetPlayer(GameContext *ctx, int socket) {
    if (ctx == NULL || ctx->player_count != 2) {
        return;
    }

    if (ctx->sockets[0] == socket) {
        ctx->p1.x = (TILE_SIZE - PLAYER_SIZE) / 2.0;
        ctx->p1.y = (WINDOW_HEIGHT / 2.0) + 20;
    }

    if (ctx->sockets[1] == socket) {
        ctx->p2.x = (TILE_SIZE - PLAYER_SIZE) / 2.0;
        ctx->p2.y = (WINDOW_HEIGHT / 2.0) - 40;
    }
}

/*
 * Movimenta o player.
 * Verifica colisão com bordas,
 * o outro player e os inimigos.
 * Se bater num inimigo, volta ao inicio.
 */
void MovePlayer(GameContext *ctx, int socket, MoveDirection direction) {
    if (ctx == NULL) {
        return;
    }

    // Player correspondente
    GameObject *player = &ctx->p1;
    if (socket == ctx->sockets[1]) {
        player = &ctx->p2;
    }

    // Movimento
    bool moved = Move(player, direction);

    // Colisão com outro player
    // Não morre, só desfaz movimento
    if (Collision(&ctx->p1, &ctx->p2)) {
        if (moved) {
            UnMove(player, direction);
        }
    }

    // Verificar colisão com inimigos
    for (int i = 0; i < NUM_ENEMIES; i++) {
        GameObject *enemy = &ctx->map.enemies[i];

        if (Collision(player, enemy)) {
            ResetPlayer(ctx, socket);
            break;
        }
    }

    // Veirifica se chegou no fim.
    int tileX = player->x / TILE_SIZE;
    if (tileX >= 0 && tileX < NUM_COLS) {
        if (ctx->map.tiles[tileX] == EndTile) {
        	ctx->winner = player->id;
        }
    }
}

/*
 * Reseta o estado inicial quando tem menos
 * de dois jogadores
 */
void RemovePlayer(GameContext *ctx, int client) {
    if (ctx == NULL || ctx->player_count == 0) {
        return;
    }

    // Remover o socket
    if (client == ctx->sockets[0]) {
        if (ctx->player_count > 1) {
            ctx->sockets[0] = ctx->sockets[1];
            ctx->p1 = ctx->p2;
        }
    } else if (client == ctx->sockets[1]) {
        ctx->sockets[1] = 0;
    } else {
        return;
    }

    ctx->player_count--;
    if (ctx->player_count < 0) {
        ctx->player_count = 0;
    }

    // Resetar jogadores existentes
    if (ctx->player_count >= 1) {
        ResetPlayer(ctx, ctx->sockets[0]);
    }

    if (ctx->player_count >= 2) {
        ResetPlayer(ctx, ctx->sockets[1]);
    }
    
    ctx->winner = -1;
}
