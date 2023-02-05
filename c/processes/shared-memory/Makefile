BIN = bin

CC = gcc
CFLAGS += -g -Wall -Werror

shmtests structarray:
	$(CC) $@.c --output $(BIN)/$@

run-structarray: structarray
	$(BIN)/structarray

clean:
	rm bin/*
	$(RM) /dev/shm/shared-memory-experiment

.PHONY: clean