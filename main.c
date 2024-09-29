#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#define UTILS_IMPL
#include "utils.h"

#define HASH_TYPE int
#define INIT_CAP 2048

int hash_function(const char* str, size_t str_len)
{
    int result = 0;
    for (int i = 0; i < str_len; ++i) {
        result = str[i]*33 + result;
    }
    return result;
}

typedef struct Cell {
    const char* key;
    HASH_TYPE value;
    bool occupied;
    struct Cell *next;
} Cell;

typedef struct {
    Cell* data;
    size_t length;
    size_t capacity;
} HashMap;

typedef struct {
    HASH_TYPE value;
    bool hasValue;
} Hash_Result;

HashMap hashmap_init(void)
{
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
HashMap hashmap_init_cap(size_t cap)
{
    HashMap result = {
        .data = NULL,
        .length = 0,
        .capacity = cap,
    };
    result.data = malloc(result.capacity * sizeof(Cell));
    assert(result.data && "Not enough memory");
    memset(result.data, 0, sizeof(Cell) * result.capacity);
    return result;
}

void hashmap_add(HashMap *hm, const char* key, HASH_TYPE value);
void hashmap_addn(HashMap *hm, const char* key, size_t key_len, HASH_TYPE value)
{
    if (hm->length >= hm->capacity) {
        // malloc?
        HashMap new = hashmap_init_cap(hm->capacity*2);
        for (size_t i = 0; i < hm->capacity; i++) {
            if (hm->data[i].occupied == true) {
                Cell* cell = &hm->data[i];
                hashmap_add(&new, cell->key, cell->value);
                while (cell->next != NULL) {
                    cell = cell->next;
                    hashmap_add(&new, cell->key, cell->value);
                }
            }
        }
        // TODO: free(hm)
        hm->data = new.data;
        hm->capacity = new.capacity;
        hm->length = new.length;
    }
    hm->length++;

    int hash = hash_function(key, key_len);
    Cell *cell = &hm->data[hash % hm->capacity];

    if (cell->occupied == true && strncmp(cell->key, key, key_len) != 0) {
        while (cell->next != NULL) {
            cell = cell->next;
        }
        cell->next = malloc(sizeof(Cell));
        assert(cell->next && "Not enough memory");
        cell = cell->next;
    }

    cell->key = key;
    cell->value = value;
    cell->occupied = true;
    cell->next = NULL;
}
void hashmap_add(HashMap *hm, const char* key, HASH_TYPE value)
{
    hashmap_addn(hm, key, strlen(key), value);
}

//TODO: hash_free()


Hash_Result hashmap_getn(HashMap *hm, const char* key, size_t key_len)
{
    int hash = hash_function(key, key_len);
    Cell* cell = &hm->data[hash % hm->capacity];
    if (cell->occupied == true) {
        while (strncmp(cell->key, key, key_len) != 0) {
            if (cell->next == NULL) break;
            cell = cell->next;
        }
        if (strncmp(cell->key, key, key_len) == 0) {
            return (Hash_Result){cell->value, true};
        }
    }
    return (Hash_Result){(HASH_TYPE)0, false};
}

Hash_Result hashmap_get(HashMap *hm, const char* key)
{
    return hashmap_getn(hm, key, strlen(key));
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        fprintf(stderr, "Error: No input file provided\n");
        printf("Usage: %s <input>\n", argv[0]);
        return 1;
    }

    HashMap hm = hashmap_init();
    const char* file_path = argv[1];
    char *content = read_file(file_path);
    CStringArray res = split_by_whitespace(content, strlen(content));
    for (size_t i = 0; i < res.length; ++i) {
        char* p = res.data[i];
        for(; *p; p++) *p = tolower(*p);
        hashmap_add(&hm, res.data[i], 1+hashmap_get(&hm, res.data[i]).value);
    }
    for (size_t i = 0; i < hm.capacity; i++) {
        if (hm.data[i].occupied == true) {
            Cell* cell = &hm.data[i];
            printf("`%s`: %d\n", cell->key, cell->value);
            while (cell->next != NULL) {
                cell = cell->next;
                printf("`%s`: %d\n", cell->key, cell->value);
            }
        }
    }

    free(res.data);
    free(content);
    return 0;
}
