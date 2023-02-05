CC := gcc
CFLAGS += -g -Wall -Werror

BIN = ./bin
SRC = ./src

TARGETS := sandwich coffee tea

# sandwich
#   depends: bread, avocado, hallumi
# coffee
#   depends: cup, ground_coffee
# tea
#   depends: cup, vounou

$(TARGETS):
	$(CC) $(SRC)/$@.c --output $(BIN)/$@

# run-$(TARGETS):
# 	./$(BIN)/$@

clean:
	$(RM) $(BIN)/$(TARGETS)

.PHONY: $(TARGETS)