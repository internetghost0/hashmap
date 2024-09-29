COMMON_CFLAGS=-Wall -Wextra -std=c99 -pedantic -ggdb

.PHONY: all
all: build

build: main.c
	$(CC) $(CFLAGS) -o main main.c

run: main.c main build
	./main ./main.c
