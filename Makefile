
CC = gcc
CFLAGS = $(shell sdl2-config --cflags)
LIBS = $(shell sdl2-config --libs) -lSDL2_ttf

all: frame_test

frame_test: frame_test.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f frame_test
