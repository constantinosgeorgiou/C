CC = g++
CFLAGS += -g -Wall -Werror

all: simplefork parent child

simplefork:
	$(CC) simplefork.cpp --output $@

parent:
	$(CC) parent.cpp --output $@

child:
	$(CC) child.cpp --output $@

memleaks:
	$(CC) $@.cpp --output $@

forkQexec:
	$(CC) $@.cpp --output $@

clean:
	$(RM) simplefork parent child memleaks forkQexec