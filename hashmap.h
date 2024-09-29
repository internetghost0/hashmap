#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#define HASH_TYPE int
#define INIT_CAP 2048


typedef struct Cell {
    const char* key;
    HASH_TYPE value;
    bool occupied;
    struct Cell *next;
} Cell;

typedef struct {
    Cell* data;
    const char** keys;
    size_t length;
    size_t capacity;
} HashMap;

typedef struct {
    HASH_TYPE value;
    bool hasValue;
} Hash_Result;

int sample_hash_func(const char* str, size_t str_len);

HashMap hashmap_init_cap(size_t cap);;
HashMap hashmap_init(void);

void hashmap_add(HashMap *hm, const char* key, HASH_TYPE value);
void hashmap_addn(HashMap *hm, const char* key, size_t key_len, HASH_TYPE value);

Hash_Result hashmap_getn(HashMap *hm, const char* key, size_t key_len);
Hash_Result hashmap_get(HashMap *hm, const char* key);

#ifdef HASHMAP_IMPL
int sample_hash_func(const char* str, size_t str_len)
{
    int result = 0;
    for (int i = 0; i < str_len; ++i) {
        result = str[i]*33 + result;
    }
    return result;
}

HashMap hashmap_init_cap(size_t cap)
{
    HashMap result = {
        .data = NULL,
        .keys = NULL,
        .length = 0,
        .capacity = cap,
    };
    result.data = malloc(result.capacity * sizeof(Cell));
    result.keys = malloc(result.capacity * sizeof(char*));
    assert(result.data && "Not enough memory");
    assert(result.keys && "Not enough memory");
    memset(result.data, 0, sizeof(Cell) * result.capacity);
    return result;
}
HashMap hashmap_init(void)
{
    return hashmap_init_cap(INIT_CAP);
}

void hashmap_addn(HashMap *hm, const char* key, size_t key_len, HASH_TYPE value)
{
    if (hm->length >= hm->capacity) {
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
    int hash = sample_hash_func(key, key_len);
    Cell *cell = &hm->data[hash % hm->capacity];

    if (cell->occupied == false) {
        hm->keys[hm->length++] = key;
    }
    else if (strncmp(cell->key, key, key_len) != 0) {
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

Hash_Result hashmap_getn(HashMap *hm, const char* key, size_t key_len)
{
    int hash = sample_hash_func(key, key_len);
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


#endif// HASHMAP_IMPL
#endif // HASHMAP_H
