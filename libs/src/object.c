#include <stdbool.h>
#include <stdlib.h>

#include "map.h"
#include "object.h"

/*
 * Detecta a colisão entre dois
 * objetos do jogo.
 * Retornar verdadeiro ou falso para
 * a colisão.
 * Melhorei: antes não pegava na parte
 * de baixo do carro, só de lado.
 */
bool Collision(GameObject *a, GameObject *b) {
    if (!a || !b)
        return false;

    float aLeft = a->x;
    float aRight = a->x + a->width;
    float aBottom = a->y;
    float aTop = a->y + a->height;

    float bLeft = b->x;
    float bRight = b->x + b->width;
    float bBottom = b->y;
    float bTop = b->y + b->height;

    return !(aLeft >= bRight || aRight <= bLeft || aBottom >= bTop ||
             aTop <= bBottom);
}

/*
 * Move um objeto baseado na direção.
 * Verifica apenas colisão com bordas.
 */
bool Move(GameObject *obj, MoveDirection direction) {
    if (obj == NULL) {
        return false;
    }

    float old_x = obj->x;
    float old_y = obj->y;

    // Movimento
    switch (direction) {
    case MoveUp:
        obj->y += TILE_SIZE;
        break;

    case MoveDown:
        obj->y -= TILE_SIZE;
        break;

    case MoveLeft:
        obj->x -= TILE_SIZE;
        break;

    case MoveRight:
        obj->x += TILE_SIZE;
        break;
    }

    // Verifica bordas
    if (obj->x < 0 || obj->y < 0 || obj->x + obj->width > WINDOW_WIDTH ||
        obj->y + obj->height > WINDOW_HEIGHT) {

        // Desfaz
        obj->x = old_x;
        obj->y = old_y;

        return false;
    }

    return true;
}

/*
 * Desfaz um movimento.
 * Não verifica nenhuma condição.
 */
void UnMove(GameObject *obj, MoveDirection direction) {
    if (obj == NULL) {
        return;
    }

    switch (direction) {
    case MoveUp:
        obj->y -= TILE_SIZE;
        break;

    case MoveDown:
        obj->y += TILE_SIZE;
        break;

    case MoveLeft:
        obj->x += TILE_SIZE;
        break;

    case MoveRight:
        obj->x -= TILE_SIZE;
        break;
    }
}
