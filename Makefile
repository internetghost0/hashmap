SHELL := /bin/bash
CFLAGS=-Wall -Wextra -std=c99 -pedantic -D_POSIX_C_SOURCE -ggdb

.PHONY: all
all: build

build: word_freq.c
	$(CC) $(CFLAGS) -O3 -o word_freq word_freq.c

run: word_freq.c word_freq build
	./word_freq ./word_freq.c

measure-time: word_freq word_freq.py shakespeare.txt
	time ./word_freq ./shakespeare.txt 1>/dev/null
	@echo "--------------------------------------------"
	time ./word_freq.py ./shakespeare.txt 1>/dev/null
