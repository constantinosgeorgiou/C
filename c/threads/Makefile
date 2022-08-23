CC = gcc
CFLAGS = -Wall -Werror

EXECUTABLES = \
							hello \
							race-condition \
							race-condition-fix \
							return \
							for-create \
							params \
							array-sum \
							trylock \
							cond-vars \
							thread-id \
							producer-consumer \
							thread-pool-api \
							thread-pool-function-pointers

hello:
	$(CC) $@.c --output $@

race-condition:
	$(CC) $@.c --output $@

race-condition-fix:
	$(CC) $@.c --output $@

$(EXECUTABLES):
	$(CC) $@.c --output $@


clean:
	$(RM) $(EXECUTABLES)

.PHONY: clean $(EXECUTABLES)