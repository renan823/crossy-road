#ifndef OBJECT_H_
#define OBJECT_H_

#include <stdbool.h>

#include "utils.h"

/*
 * Definições dos tipos e funções
 * para criar objetos no jogo.
 */

 
/*
 * Define o tipo GameObject, que nesse caso
 * será sempre um quadrado ou retângulo.
 * O GameObject representa tanto os players
 * como os inimos (carros).
 * O player possui o movimento controlado pelo
 * jogador, enquanto os inimigos possuem
 * movimentação automática.
 */
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

/*
 * Define os eventos do jogo.
 */
typedef enum game_event GameEvent;
enum game_event { NullEvent, DeadEvent, MoveEvent, WinEvent };

typedef GameEvent ObjectUpdater(GameObject *, void *);

/*
 * Define os tipos de movimento que um jogado pode fazer.
 */
typedef enum move_direction MoveDirection;
enum move_direction { MoveUp, MoveDown, MoveLeft, MoveRight };

GameObject *NewGameObject(int id, int pos_x, int pos_y, int width, int height,
                          double velocity, HexColor color);
void DestroyGameObject(GameObject **obj);

bool Collision(GameObject *obj1, GameObject *obj2);

GameEvent UpdateObject(GameObject *obj, void *data, ObjectUpdater *updater);

#endif
