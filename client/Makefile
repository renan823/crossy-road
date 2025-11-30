CC = gcc
CFLAGS = -Wall -std=c2x -O2 -I../libs/include

# Bibliotecas internas usadas pelo cliente
CLIENT_LIBS = \
	../libs/src/semaphore.c \
	../libs/src/threads.c \
	../libs/src/object.c \
	../libs/src/game.c \
	../libs/src/map.c \
	../libs/src/net.c \

# Linkagem do OpenGL
GL_FLAGS = -lglut -lGL -lGLU -lm

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

TARGET = client

all: $(TARGET)

$(TARGET): $(OBJ) $(CLIENT_LIBS)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(CLIENT_LIBS) $(GL_FLAGS)

clean:
	rm -f *.o $(TARGET)