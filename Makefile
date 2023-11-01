COMMON_CFLAGS=		-Wall -Wextra -std=c99 -pedantic

.PHONY: all
all: run

build: main.c
	$(CC) $(CFLAGS) -o main main.c

run: main.c main
	./main ./main.c
