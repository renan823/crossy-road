#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// Tela
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

// Faixas de terreno
#define TILE_SIZE 30  
#define NUM_COLS (WINDOW_WIDTH / TILE_SIZE)

#define PLAYER_SIZE 20 

// Dimensões do Carro
#define CAR_WIDTH 22
#define CAR_HEIGHT 45

// Identificação terreno
#define TERRAIN_GRASS 0
#define TERRAIN_ASPHALT 1

// Fiz hard coded para ter mais controle
int map_layout[NUM_COLS] = {
    0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 
    1, 1, 0, 1, 0, 0, 1, 1, 1, 0
};

// Estrutura simples para o jogador (depois mudamos)
typedef struct {
    float x, y;
    float r, g, b; // Cor
} Player;

// Estrutura para os carros
typedef struct {
    float x, y;
    float width, height;
    float r, g, b;
    int active;
    float velocity;
} Car;

Player p1, p2;
Car cars[100]; // Limite arbitrário de carros
int total_cars = 0;

// Função para gerar os carros no asfalto
void spawnCars() {
    total_cars = 0;
    srand(time(NULL)); // Inicializa a semente aleatória

    for (int i = 0; i < NUM_COLS; i++) {
        // Apenas gera carros se o terreno for asfalto
        if (map_layout[i] == TERRAIN_ASPHALT) {
            
            // Cria 2 carros por faixa
            for (int k = 0; k < 2; k++) {
                Car *c = &cars[total_cars];
                
                // Centraliza na faixa horizontalmente
                c->x = (i * TILE_SIZE) + ((TILE_SIZE - CAR_WIDTH) / 2.0);
                
                // Posição vertical aleatória
                c->y = rand() % (WINDOW_HEIGHT - CAR_HEIGHT);
                
                // Velocidade
                int v = 0.01f + (rand() % 8) * 0.01f;
                if (rand() % 2 == 0) {
                	c->velocity = -0.1;
                } else {
                	c->velocity = 0.1;
                }
                
                c->width = CAR_WIDTH;
                c->height = CAR_HEIGHT;
                c->active = 1;

                // Cor vermelha
                c->r = 1.0; c->g = 0.0; c->b = 0.0; 

                total_cars++;
            }
        }
    }
}

void idle() {
    glutPostRedisplay();
}

// Função de inicialização da janela do OpenGL
void init() {
    // Cor de fundo, fiz bem vibrante para a gente perceber fácil (Rosa)
    glClearColor(1.0, 0.41, 0.71, 1.0);

    // O OpenGL tem coordenadas diferentes, o (0,0) é na esquerda em cima
    // Esse trecho deixa as coordenadas do jeito que estamos acostumados
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
    glMatrixMode(GL_MODELVIEW);

    // Gera os carros
    spawnCars();
}

// --- Funções Auxiliares de Desenho ---
void drawRect(float x, float y, float width, float height, float r, float g, float b) {
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

void drawDashedLine(float x, float y1, float y2) {
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

// --- Função Principal de Desenho (Display) ---
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // 1. Desenha Mapa
    for (int i = 0; i < NUM_COLS; i++) {
        float x_pos = i * TILE_SIZE;
        
        if (map_layout[i] == TERRAIN_GRASS) {
            // Grama
            drawRect(x_pos, 0, TILE_SIZE, WINDOW_HEIGHT, 0.0, 0.5, 0.0);
        } else {
            // Asfalto
            drawRect(x_pos, 0, TILE_SIZE, WINDOW_HEIGHT, 0.3, 0.3, 0.3);
            
            // Desenhar linha amarela pontilhada no meio da faixa
            drawDashedLine(x_pos + (TILE_SIZE / 2), 0, WINDOW_HEIGHT);
        }
    }

    // 2. Desenhar Carros (Novo)
    for (int i = 0; i < total_cars; i++) {
    	cars[i].y += cars[i].velocity;
    
   		// Verificar se saiu da tela
        if (cars[i].y + cars[i].height < 0 || cars[i].y > WINDOW_HEIGHT) {
       		if (cars[i].velocity > 0) {
         		cars[i].y = -cars[i].height;
            } else {
                cars[i].y = WINDOW_HEIGHT;
            }
        }
        
       
        
        cars[i].active = 1;
    
        if (cars[i].active) {
            drawRect(cars[i].x, cars[i].y, cars[i].width, cars[i].height, 
                     cars[i].r, cars[i].g, cars[i].b);
        }
    }

    // 3. Desenhar os Jogadores (Renumerado)
    drawRect(p1.x, p1.y, PLAYER_SIZE, PLAYER_SIZE, p1.r, p1.g, p1.b);
    drawRect(p2.x, p2.y, PLAYER_SIZE, PLAYER_SIZE, p2.r, p2.g, p2.b);

    glFlush(); // Envia os comandos para a GPU
}

int main(int argc, char** argv) {
    // Setup inicial dos jogadores
    // P1 (Branco)
    p1.x = (TILE_SIZE - PLAYER_SIZE) / 2.0; // Centralizado na faixa 0
    p1.y = (WINDOW_HEIGHT / 2.0) + 20;
    p1.r = 1.0; p1.g = 1.0; p1.b = 1.0;
    
    

    // P2 (Preto)
    p2.x = (TILE_SIZE - PLAYER_SIZE) / 2.0; // Centralizado na faixa 0
    p2.y = (WINDOW_HEIGHT / 2.0) - 40;
    p2.r = 0.0; p2.g = 0.0; p2.b = 0.0;

    // Configuração do GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Crossy Road - Multiplayer");

    init(); 

    glutDisplayFunc(display);
    glutIdleFunc(idle);
    
    glutMainLoop();
    return 0;
}