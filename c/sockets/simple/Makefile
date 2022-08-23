CC = gcc
CFLAGS = -Wall -Werror

server:
	$(CC) $@.c --output $@

client:
	$(CC) $@.c --output $@

clean:
	$(RM) $(EXECUTABLES) server client

.PHONY: clean $(EXECUTABLES) server client