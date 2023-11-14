CC=gcc
OPT_REL=-Ofast
OPT_DEB=-O0 -ggdb3# -fsanitize=address
CFLAGS=$(OPT_DEB) -Wall -Wextra -Werror -Wno-strict-aliasing

SRC=$(wildcard src/*.c)
OBJ=$(patsubst src/%.c,%.o,$(SRC))
LIB=-L/usr/local/lib -lassimp -lm
INC=-Iinclude
BIN=mdlxtractor

BETTY_SCAN=src/*.c include/*.h

default: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LIB)

%.o: src/%.c
	$(CC) $(CFLAGS) -c $^ $(INC)

clean:
	rm -rf $(BIN) *.scn *.o

betty-style:
	betty-style --allow-global-variables $(BETTY_SCAN)

betty-doc:
	betty-doc $(BETTY_SCAN)

betty:
	clear; make betty-doc; make betty-style;
