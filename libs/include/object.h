#ifndef OBJECT_H_
#define OBJECT_H_

#include <stdbool.h>

#define ENEMY_WIDTH 22
#define ENEMY_HEIGHT 45

#define PLAYER_SIZE 20 

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
    float x;
    float y;
    float width;
    float height;
    float velocity;
    float r, g, b;
};

/*
 * Define os tipos de movimento que um jogado pode fazer.
 */
typedef enum move_direction MoveDirection;
enum move_direction { MoveUp, MoveDown, MoveLeft, MoveRight };

typedef enum event_type EventType;
enum event_type { ConnectEvent, DisconnecEvent, MoveEvent };

/*
 * Define os eventos do jogo.
 */
typedef struct game_event GameEvent;
struct game_event {
	MoveDirection direction;
	EventType type;
};

bool Collision(GameObject *obj1, GameObject *obj2);

bool Move(GameObject *obj, MoveDirection direction);
void UnMove(GameObject *obj, MoveDirection direction);

#endif
