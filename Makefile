CFLAGS=-Wall -Wextra -std=c11 -pedantic -ggdb $(shell sdl2-config --cflags)
LIBS=$(shell sdl2-config --libs) -lSDL2_ttf -lm

zet: src/main.c src/vector.c src/font.c
	$(CC) $(CFLAGS) -Isrc -o ./zet src/main.c src/vector.c src/font.c src/rope.c $(LIBS)

clean:
	rm -f ./zet

.PHONY: clean
