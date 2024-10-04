SHELL := /bin/bash
CFLAGS=-Wall -Wextra -std=c99 -pedantic -D_POSIX_C_SOURCE -ggdb

.PHONY: all build run measure-time
all: build

build: word_freq.c
	$(CC) $(CFLAGS) -O3 -o word_freq word_freq.c

run: word_freq.c word_freq build
	./word_freq ./word_freq.c

measure-time: word_freq word_freq.py shakespeare.txt
	time ./word_freq ./shakespeare.txt 1>/dev/null
	@echo "--------------------------------------------"
	time ./word_freq.py ./shakespeare.txt 1>/dev/null
	@echo
	@if diff <(./word_freq ./shakespeare.txt) <(./word_freq.py ./shakespeare.txt) > /dev/null; then \
		echo "Outputs are the same"; \
	else \
		diff <(./word_freq ./shakespeare.txt) <(./word_freq.py ./shakespeare.txt); \
		echo "Outputs are different"; \
	fi

