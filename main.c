#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#define UTILS_IMPL
#include "utils.h"


typedef struct {
    const char* key;
    int value;
    bool occupied;
} Cell;

typedef struct {
    Cell* data;
    size_t length;
    size_t capacity;
} HashMap;

HashMap hashmap_init() {
    const size_t INIT_CAP = 10000;
    HashMap result = {
        .data = NULL,
        .length = 0,
        .capacity = INIT_CAP,
    };
    result.data = malloc(result.capacity * sizeof(Cell));
    assert(result.data && "Not enough memory");
    memset(result.data, 0, sizeof(Cell) * result.capacity);
    return result;
}


int hash(const char* str, size_t str_len) {
    int result = 0;
    for (int i = 0; i < str_len; ++i) {
        result = str[i]*33 + result;
    }
}


int main(int argc, char** argv)
{
    if (argc != 2) {
        fprintf(stderr, "Error: No input file provided\n");
        printf("Usage: %s <input>\n", argv[0]);
        return 1;
    }
    const char* file_path = argv[1];
    char *content = read_file(file_path);
    CStringArray res = split_by_space(content, strlen(content));
    for (size_t i = 0; i < res.length; ++i) {
        printf("`%s`\n", res.data[i]);
    }
    free(res.data);
    free(content);
    return 0;
}
