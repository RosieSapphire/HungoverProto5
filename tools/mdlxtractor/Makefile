CC=gcc
OPT_REL=-Ofast
OPT_DEB=-O0 -ggdb3# -fsanitize=address
CFLAGS=$(OPT_DEB) -Wall -Wextra -Werror -Wno-strict-aliasing

SRC=$(wildcard src/*.c)
OBJ=$(patsubst src/%.c,%.o,$(SRC))
LIB=-L/usr/local/lib -lassimp -lm -lpthread
INC=-Iinclude
BIN=mdlxtractor

default: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LIB)

%.o: src/%.c
	$(CC) $(CFLAGS) -c $^ $(INC)

clean:
	rm -rf $(BIN) *.scn *.mdl *.o

BETTY_SCAN=include/*.h src/*.c

betty-style:
	betty-style --subjective --strict \
		--allow-global-variables \
		--ignore=CAMELCASE,PREFER_KERNEL_TYPES $(BETTY_SCAN)

betty:
	clear; make betty-doc; make betty-style;
