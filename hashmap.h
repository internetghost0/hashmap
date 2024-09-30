#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#define HASH_TYPE int
#define INIT_CAP 1024


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

// TODO: Hash_Result hashmap_pop()

void hashmap_free(HashMap* hm);

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
    assert(result.data && "Not enough memory");
    result.keys = malloc(result.capacity * sizeof(char*));
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
    if (hm->capacity <= 0) hm->capacity = 1;
    if (hm->length >= hm->capacity) {
        HashMap new = hashmap_init_cap(hm->capacity*2);
        for (size_t i = 0; i < hm->length; i++) {
            hashmap_add(&new, hm->keys[i], hashmap_get(hm, hm->keys[i]).value);
        }
        hashmap_free(hm);

        hm->data = new.data;
        hm->capacity = new.capacity;
        hm->length = new.length;
        hm->keys = new.keys;
    }
    int hash = sample_hash_func(key, key_len);
    Cell *cell = &hm->data[hash % hm->capacity];

    // if cell unoccupied -> add new key
    if (cell->occupied == false) {
        hm->keys[hm->length++] = key;
    }
    else if (strncmp(cell->key, key, key_len) != 0) {
        while (cell->next != NULL ) {
            cell = cell->next;
            if (strncmp(cell->key, key, key_len) == 0) break;
        }
        // if key is new -> allocate a cell and add new key
        if (strncmp(cell->key, key, key_len) != 0) {
            cell->next = malloc(sizeof(Cell));
            assert(cell->next && "Not enough memory");
            cell = cell->next;
            hm->keys[hm->length++] = key;
        }
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

void hashmap_free(HashMap* hm)
{
    for (size_t i = 0; i < hm->capacity; ++i) {
        Cell* next = hm->data[i].next;
        while (next != NULL) {
            Cell* t = next;
            next = next->next;
            free(t);
        }
    }
    free(hm->keys);
    hm->keys = NULL;
    free(hm->data);
    hm->data = NULL;
    hm = NULL;
}

#endif// HASHMAP_IMPL
#endif // HASHMAP_H
