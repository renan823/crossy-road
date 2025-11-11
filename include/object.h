#ifndef OBJECT_H_
#define OBJECT_H_

#include <stdbool.h>

#include "utils.h"

typedef struct game_object GameObject;
struct game_object {
    int id;
    int pos_x;
    int pos_y;
    int width;
    int height;
    double velocity;
    HexColor color;
};

typedef void ObjectUpdater(GameObject*, void*);

/*
 *
 */
typedef enum game_event GameEvent;
enum game_event { NullEvent, DeadEvent, MoveEvent, WinEvent };

/*
 * 
 */
typedef enum move_direction MoveDirection;
enum move_direction { MoveUp, MoveDown, MoveLeft, MoveRight };

bool Collision(GameObject *obj1, GameObject *obj2);

GameEvent UpdateObject(GameObject *obj, void *data, ObjectUpdater *updater);

#endif
