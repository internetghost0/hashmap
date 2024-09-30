SHELL := /bin/bash
COMMON_CFLAGS=-Wall -Wextra -std=c99 -pedantic -ggdb

.PHONY: all
all: build

build: main.c
	$(CC) $(CFLAGS) -O3 -o main main.c

run: main.c main build
	./main ./main.c

measure-time: main main.py
	time ./main.py ./shakespeare.txt

	time ./main ./shakespeare.txt
