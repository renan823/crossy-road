#ifndef SCREEN_H_
#define SCREEN_H_

#include "game.h"
#include "map.h"

void InitScreen(int argc, char *argv[], void (*display) (void));
void ScreenLoop(void);

void FlushScreen(void);
void ClearScreen(void);

void DrawMap(GameMap *map);
void DrawEnemies(GameMap *map);
void DrawPlayers(GameContext *ctx);

#endif