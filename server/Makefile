CC = gcc
CFLAGS = -Wall -std=c2x -O2 -I../libs/include

SERVER_LIBS = \
	../libs/src/semaphore.c \
	../libs/src/threads.c \
	../libs/src/object.c \
	../libs/src/game.c \
	../libs/src/map.c \
	../libs/src/net.c \

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

TARGET = server

all: $(TARGET)

$(TARGET): $(OBJ) $(SERVER_LIBS)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(SERVER_LIBS)

clean:
	rm -f *.o $(TARGET)

run:
	./$(TARGET)