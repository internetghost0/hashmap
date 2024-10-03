#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#define HASH_VALUE_TYPE int64_t
#define HASH_KEY_TYPE   const char*

#define INIT_CAP 2048

typedef struct Cell {
    HASH_KEY_TYPE key;
    HASH_VALUE_TYPE value;
    bool occupied;
    struct Cell *next;
} Cell;

typedef struct {
    Cell* data;
    HASH_KEY_TYPE* keys;
    size_t length;
    size_t capacity;
} HashMap;

typedef struct {
    HASH_VALUE_TYPE value;
    bool hasValue;
} HM_Result;

typedef struct {
    HASH_KEY_TYPE key;
    HASH_VALUE_TYPE value;
} HM_Pair;



uint64_t sample_hash_func(const char *str);

HashMap *hashmap_init_cap(size_t cap);
HashMap *hashmap_init(void);

bool hashmap_contains_key(HashMap *hm, HASH_KEY_TYPE key);

void hashmap_add(HashMap *hm, HASH_KEY_TYPE key, HASH_VALUE_TYPE value);

HM_Result hashmap_get(HashMap *hm, HASH_KEY_TYPE key);

HM_Result hashmap_pop(HashMap *hm, HASH_KEY_TYPE key);

HM_Pair* hashmap_to_pairs(HashMap *hm);

void hashmap_free(HashMap* hm);

#ifdef HASHMAP_IMPL
uint64_t sample_hash_func(const char *str)
{
    uint64_t hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash;
}


HashMap *hashmap_init_cap(size_t cap)
{

    HashMap *result = malloc(sizeof(HashMap));
    *result = (HashMap){
        .data = NULL,
        .keys = NULL,
        .length = 0,
        .capacity = cap,
    };
    result->data = malloc(result->capacity * sizeof(Cell));
    assert(result->data && "Not enough memory");
    result->keys = malloc(result->capacity * sizeof(char*));
    assert(result->keys && "Not enough memory");
    memset(result->data, 0, sizeof(Cell) * result->capacity);
    return result;
}
HashMap *hashmap_init(void)
{
    return hashmap_init_cap(INIT_CAP);
}

bool hashmap_contains_key(HashMap *hm, HASH_KEY_TYPE key)
{
    for (size_t i = 0; i < hm->length; ++i) {
        if (strcmp(hm->keys[i], key) == 0) return true;
    }
    return false;
}

void hashmap_add(HashMap *hm, HASH_KEY_TYPE key, HASH_VALUE_TYPE value)
{
    if (hm->capacity <= 0) hm->capacity = 1;

    if (hm->length >= hm->capacity) {
        HashMap *new = hashmap_init_cap(hm->capacity*2);
        for (size_t i = 0; i < hm->length; i++) {
            hashmap_add(new, hm->keys[i], hashmap_get(hm, hm->keys[i]).value);
        }
        hashmap_free(hm);
        hm = new;
    }
    int hash = sample_hash_func(key);
    Cell *cell = &hm->data[hash % hm->capacity];

    // if cell unoccupied -> add new key
    if (cell->occupied == false) {
        hm->keys[hm->length++] = key;
        cell->key = key;
        cell->value = value;
        cell->occupied = true;
        cell->next = NULL;
    }
    // if cell occupied & key is same
    else if (strcmp(cell->key, key) == 0) {
        cell->value = value;
    }
    // if cell occupied & key is different
    else {
        // iterate through linked list while end or while find the key
        while (cell->next != NULL ) {
            if (strcmp(cell->key, key) == 0) break;
            cell = cell->next;
        }
        // if find the key -> update the value
        if (strcmp(cell->key, key) == 0) {
            cell->value = value;
        // else -> create a new cell
        } else {
            cell->next = malloc(sizeof(Cell));
            assert(cell->next && "Not enough memory");
            cell = cell->next;
            hm->keys[hm->length++] = key;

            cell->key = key;
            cell->value = value;
            cell->occupied = true;
            cell->next = NULL;
        }
    }
}

HM_Result hashmap_get(HashMap *hm, HASH_KEY_TYPE key)
{
    int hash = sample_hash_func(key);
    Cell* cell = &hm->data[hash % hm->capacity];
    if (cell->occupied == true) {
        while (strcmp(cell->key, key) != 0) {
            if (cell->next == NULL) break;
            cell = cell->next;
        }
        if (strcmp(cell->key, key) == 0) {
            return (HM_Result){cell->value, true};
        }
    }
    return (HM_Result){(HASH_VALUE_TYPE)0, false};
}

HM_Pair* hashmap_to_pairs(HashMap *hm)
{
    HM_Pair* result = malloc(hm->length * sizeof(HM_Pair));
    for (size_t i = 0; i < hm->length; ++i) {
        result[i] = (HM_Pair) {
            hm->keys[i],
            hashmap_get(hm, hm->keys[i]).value,
        };
    }
    return result;
}

HM_Result hashmap_pop(HashMap *hm, HASH_KEY_TYPE key)
{
    HM_Result result = (HM_Result){0};

    int hash = sample_hash_func(key);
    Cell* cell = &hm->data[hash % hm->capacity];

    if (cell->occupied == true) {
        if (strcmp(cell->key, key) == 0) {
            result.value = cell->value;
            result.hasValue = true;

            if (cell->next != NULL) {
                Cell *t = cell->next;
                // copy
                cell->key = t->key;
                cell->value = t->value;
                cell->occupied = t->occupied;
                cell->next = t->next;

                free(t);
            } else {
                *cell = (Cell){0};
            }
        } else {
            while (cell->next != NULL) {
                if (strcmp(cell->next->key, key) == 0) break;
                cell = cell->next;
            }
            if (strcmp(cell->next->key, key) == 0) {
                result.value = cell->next->value;
                result.hasValue = true;
                Cell *t = cell->next;
                cell->next = t->next;
                free(t);
            }
        }
    }
    if (result.hasValue) {
        // remove from keys
        HASH_KEY_TYPE* keys = malloc(hm->capacity * sizeof(char*));
        size_t length = 0;
        for (size_t i = 0; i < hm->length; ++i) {
            if (strcmp(hm->keys[i], key) != 0) {
                keys[length++] = hm->keys[i];
            }
        }
        free(hm->keys);
        hm->keys = keys;
        hm->length = length;
    }
    return result;
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
    free(hm);
    hm = NULL;
}

#endif// HASHMAP_IMPL
#endif // HASHMAP_H
