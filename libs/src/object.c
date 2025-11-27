#include <stdbool.h>
#include <stdlib.h>

#include "object.h"

/*
 * Inicializa um novo GameObject.
 */
 GameObject *NewGameObject(int id, float x, float y, float w, float h, float v, float r, float g, float b) {
    GameObject *obj = (GameObject *)malloc(sizeof(GameObject));
    if (obj == NULL) {
        return NULL;
    }
    
    obj->id = id;
    obj->x = x;
    obj->y = y;
    obj->width = w;
    obj->height = h;
    obj->velocity = v;
    obj->r = r;
    obj->g = g;
    obj->b = b;

    return obj;
}

/*
 * Desaloca um GameObject.
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

    bool collisionX = obj1->x + obj1->width >= obj2->x &&
                      obj2->x + obj2->width >= obj1->x;

    bool collisionY = obj1->y + obj1->height >= obj2->y &&
                      obj2->y + obj2->height >= obj1->y;

    return collisionX && collisionY;
}