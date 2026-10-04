CC=gcc
CFLAGS=-O2 -Wall -std=c99
LFLAGS=

ifeq ($(OS),Windows_NT)
	BIN=sortvis.exe
	TEST_BIN=sortvis_test.exe
	TEST_RUN=$(TEST_BIN)
	RM=del
	LFLAGS+=-s
else
	BIN=sortvis
	TEST_BIN=sortvis_test
	TEST_RUN=./$(TEST_BIN)
	RM=rm -f
endif

all: $(BIN)

$(BIN): sortvis.c sortvis.h algs.h helpers.h vt.h
	$(CC) $(CFLAGS) $< -o $(BIN) $(LFLAGS)

$(TEST_BIN): test.c sortvis.h algs.h helpers.h vt.h
	$(CC) $(CFLAGS) $< -o $(TEST_BIN) $(LFLAGS)

test: $(TEST_BIN)
	$(TEST_RUN)

clean:
	$(RM) $(BIN) $(TEST_BIN)

.PHONY: all test clean
