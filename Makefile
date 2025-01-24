CFLAGS=-Wall -Wextra -std=c11 -pedantic -ggdb `pkg-config --cflags sdl2`
LIBS=`pkg-config --libs sdl2`

zet: src/main.c
	$(CC) $(CFLAGS) -o ./bin/zet src/main.c $(LIBS) -lm
