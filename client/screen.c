#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>

#include "../libs/include/game.h"
#include "../libs/include/map.h"
#include "../libs/include/object.h"

#include "screen.h"

/*
 * Desenh um retangulo
 */
void _DrawRect(float x, float y, float width, float height, float r, float g, float b) {
	// Desenha um retângulo que "começa" em (x, y)

    glColor3f(r, g, b);

    glBegin(GL_POLYGON);
        // O glBegin é como se fosse um loop, por isso a indentação
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
    glEnd();
}

/*
 * Desenha linha pontilhada
 */
void _DrawDashedLine(float x, float y1, float y2) {
	glColor3f(1.0, 0.8, 0.0); // Amarelo
    glLineWidth(2.0);
    
    // Ativa o pontilhado
    glEnable(GL_LINE_STIPPLE);
    // Padrão de pontilhado (bits: 0000 0000 1111 1111)
    glLineStipple(2, 0x00FF); 
    
    glBegin(GL_LINES);
        glVertex2f(x, y1);
        glVertex2f(x, y2);
    glEnd();
    
    glDisable(GL_LINE_STIPPLE);
}

/*
 * Escreve um texto na tela
 */
void _DrawText(float x, float y, const char *text) {
    glColor3f(1.0, 1.0, 1.0); // branco

    glRasterPos2f(x, y);

    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text);
        text++;
    }
}


/*
 * Define a função idle do opengl
 */
void _Idle(void) {
	 glutPostRedisplay();
}

/*
 * Inicia a janela do opengl
 */
void InitScreen(int argc, char *argv[], void (*display) (void)) {
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Crossy Road - Multiplayer");

    // Cor de fundo, fiz bem vibrante para a gente perceber fácil (Rosa)
    glClearColor(1.0, 0.41, 0.71, 1.0);

    // O OpenGL tem coordenadas diferentes, o (0,0) é na esquerda em cima
    // Esse trecho deixa as coordenadas do jeito que estamos acostumados
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    
    // Defien o display
    glutDisplayFunc(display);
    glutIdleFunc(_Idle);
}

/*
 * Define a função que lida com teclas.
 */
void SetScreenKeybinds(void (*func)(unsigned char key, int x, int y)) {
	glutKeyboardFunc(func);
}

/*
 * Manda pra GPU
 */
void FlushScreen(void) {
	glutSwapBuffers();
}

/*
 * Limpa a tela
 */
void ClearScreen(void) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

/*
 * Inicia o loop do opengl
 */
void ScreenLoop(void) {
	glutMainLoop();
}

/*
 * Desenha o mapa
 */
void DrawMap(GameMap *map) {
	if (map == NULL) {
		return;
	}
	
	for (int i = 0; i < NUM_COLS; i++) {
        float x_pos = i * TILE_SIZE;
        
        if (map->tiles[i] == RoadTile) {
	       	// Asfalto + linha amarela
	        _DrawRect(x_pos, 0, TILE_SIZE, WINDOW_HEIGHT, 0.3, 0.3, 0.3);
	        _DrawDashedLine(x_pos + (TILE_SIZE / 2), 0, WINDOW_HEIGHT);
        } else {
	        // Grama
	        _DrawRect(x_pos, 0, TILE_SIZE, WINDOW_HEIGHT, 0.0, 0.5, 0.0);
        }
    }
}

/*
 * Desenha os inimigos
 */
void DrawEnemies(GameMap *map) {
	if (map == NULL) {
		return;
	}
	
	for (int i = 0; i < NUM_ENEMIES; i++) {
		GameObject *enemy = &map->enemies[i];
		
	 	_DrawRect(enemy->x, enemy->y, enemy->width, enemy->height, 
				enemy->r, enemy->g, enemy->b);
	}
}

/*
 * Desenha os players
 */
void DrawPlayers(GameContext *ctx) {
	if (ctx == NULL) {
		return;
	}
	
	_DrawRect(ctx->p1.x, ctx->p1.y, PLAYER_SIZE, PLAYER_SIZE, ctx->p1.r, ctx->p1.g, ctx->p1.b);
    _DrawRect(ctx->p2.x, ctx->p2.y, PLAYER_SIZE, PLAYER_SIZE, ctx->p2.r, ctx->p2.g, ctx->p2.b);
}

/*
 * Tela de loading enquanto
 * espera por outro player.
 */
void DrawLoading(void) {
	char msg[] = "Aguardando outro jogador...";
    float x = WINDOW_WIDTH / 2 - 100;
    float y = WINDOW_HEIGHT / 2;
    
    _DrawText(x, y, msg);
}

/*
 * Tela de vitória
 */
void DrawWin(void) {
	char msg[] = "Voce venceu!";
    float x = WINDOW_WIDTH / 2 - 100;
    float y = WINDOW_HEIGHT / 2;
    
    _DrawText(x, y, msg);
}

/*
 * Tela de derrota.
 */
void DrawLose(void) {
	char msg[] = "Voce perdeu!";
    float x = WINDOW_WIDTH / 2 - 100;
    float y = WINDOW_HEIGHT / 2;
    
    _DrawText(x, y, msg);
}