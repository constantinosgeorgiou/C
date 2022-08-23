CC = gcc
CFLAGS += -g -Wall -Werror

all: parent child

run: parent child
	./parent

parent:
	$(CC) $@.c --output $@

child:
	$(CC) $@.c --output $@

clean:
	$(RM) parent child

.PHONY: all run clean