CC=gcc
CFLAGS=-O3 --std=c99 -Wall
LFLAGS=

ifeq ($(OS),Windows_NT)
	BIN=sortvis-win32.exe
	RM=del
	LFLAGS+=-s
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		BIN=sortvis-linux
	endif
	ifeq ($(UNAME_S),Darwin)
		BIN=sortvis-darwin
	endif
	RM=rm -f
endif

all: sortvis

sortvis: sortvis.c
	$(CC) $(CFLAGS) $< -o $(BIN) $(LFLAGS)

clean:
ifeq ($(OS),Windows_NT)
	$(RM) $(BIN)
else
	$(RM) $(BIN)
endif