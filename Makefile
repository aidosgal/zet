CFLAGS=-Wall -Wextra -std=c11 -pedantic -ggdb

zet: src/main.c
	$(CC) $(CFLAGS) -o ./bin/zet src/main.c
