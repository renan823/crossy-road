#include <stdbool.h>
#include <stdlib.h>

#include "object.h"

/*
 * Detecta a colisão entre dois
 * objetos do jogo.
 * Retornar verdadeiro ou falso para
 * a colisão.
 */
bool Collision(GameObject *obj1, GameObject *obj2) {
    if (obj1 == NULL || obj2 == NULL) {
        return false;
    }

    bool collisionX = obj1->pos_x + obj1->width >= obj2->pos_x &&
                      obj2->pos_x + obj2->width >= obj1->pos_x;

    bool collisionY = obj1->pos_y + obj1->height >= obj2->pos_y &&
                      obj2->pos_y + obj2->height >= obj1->pos_y;
                      
    return collisionX && collisionY;
}

/*
 *
 */
GameEvent UpdateObject(GameObject *obj, void *data, ObjectUpdater *updater) {
	return NullEvent;
}
