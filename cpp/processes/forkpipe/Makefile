CC = g++
CFLAGS += -g -Wall -Werror

all: parent child

run: parent child
	./parent

parent:
	$(CC) $@.cpp --output $@

child:
	$(CC) $@.cpp --output $@

clean:
	$(RM) parent child

.PHONY: all run clean