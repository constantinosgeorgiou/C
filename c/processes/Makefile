BIN = bin

CC = gcc
CFLAGS += -g -Wall -Werror

intro:
	gcc -o $(BIN)/intro 00-intro.c
waiting:
	gcc -o $(BIN)/waiting 01-waiting.c
ids:
	gcc -o $(BIN)/ids 02-ids.c
multifork:
	gcc -o $(BIN)/multifork 03-multifork.c

forkfunc:
	$(CC) $@.c --output $(BIN)/$@


run-:

clean:
	rm bin/*

.PHONY: clean