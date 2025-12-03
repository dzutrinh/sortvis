CC=gcc
CFLAGS=-O2 -Wall -std=c99
LFLAGS=

ifeq ($(OS),Windows_NT)
	BIN=sortvis.exe
	RM=del
	LFLAGS+=-s
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		BIN=sortvis
	endif
	ifeq ($(UNAME_S),Darwin)
		BIN=sortvis
	endif
	RM=rm -f
endif

all: sortvis

sortvis: sortvis.c
	$(CC) $(CFLAGS) $< -o $(BIN) $(LFLAGS)

test: test.c
	$(CC) $(CFLAGS) $< -o test $(LFLAGS)

clean:
ifeq ($(OS),Windows_NT)
	$(RM) $(BIN)
else
	$(RM) $(BIN)
endif
