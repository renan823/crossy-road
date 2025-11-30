#ifndef SCREEN_H_
#define SCREEN_H_

#include "game.h"
#include "map.h"

void InitScreen(int argc, char *argv[], void (*display) (void));
void SetScreenKeybinds(void (*func)(unsigned char key, int x, int y));
void ScreenLoop(void);

void FlushScreen(void);
void ClearScreen(void);

void DrawMap(GameMap *map);
void DrawEnemies(GameMap *map);
void DrawPlayers(GameContext *ctx);
void DrawLoading(void);
void DrawWin(void);
void DrawLose(void);

#endif