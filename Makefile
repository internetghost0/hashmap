SHELL := /bin/bash
CFLAGS=-Wall -Wextra -std=c99 -pedantic -D_POSIX_C_SOURCE -ggdb

.PHONY: all
all: build

build: main.c
	$(CC) $(CFLAGS) -O3 -o main main.c

run: main.c main build
	./main ./main.c

measure-time: main main.py shakespeare.txt
	time ./main ./shakespeare.txt 1>/dev/null
	@echo "--------------------------------------------"
	time ./main.py ./shakespeare.txt 1>/dev/null
