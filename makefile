CC=gcc
CFLAGS=-O2 -Wall -std=c99
LFLAGS=

ifeq ($(OS),Windows_NT)
	BIN=sortvis.exe
	RM=del
	LFLAGS+=-s
else
	BIN=sortvis
	RM=rm -f
endif

all: $(BIN)

$(BIN): sortvis.c sortvis.h algs.h helpers.h vt.h
	$(CC) $(CFLAGS) $< -o $(BIN) $(LFLAGS)

clean:
	$(RM) $(BIN)

.PHONY: all clean
