#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "object.h"

/*
 * Inicializa um novo GameObject.
 */
GameObject *NewGameObject(int id, int pos_x, int pos_y, int width, int height,
                          double velocity, HexColor color) {
    GameObject *obj = (GameObject *)malloc(sizeof(GameObject));
    if (obj == NULL) {
        return NULL;
    }
    
    obj->id = id;
    obj->pos_x = pos_x;
    obj->pos_y = pos_y;
    obj->width = width;
    obj->height = height;
    obj->velocity = velocity;
    strcpy(obj->color, color);

    return obj;
}

/*
 * desaloca um GameObject.
 */
void DestroyGameObject(GameObject **obj) {
	if (*obj == NULL) {
		return;
	}
	
	free(*obj);
	*obj = NULL;
}

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
 * Executa uma função para atualizar algum dado interno do
 * objeto, podendo atuar em movimentação, colisão, etc.
 */
GameEvent UpdateObject(GameObject *obj, void *data, ObjectUpdater *updater) {
    if (obj == NULL) {
        return NullEvent;
    }

    return updater(obj, data);
}
