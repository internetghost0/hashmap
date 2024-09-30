#ifndef UTILS_H_
#define UTILS_H_
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#define SPLIT_CAPACITY 2048

typedef struct {
    char** data;
    size_t length;
    size_t capacity;
} CStringArray;

typedef struct {
    char *data;
    size_t length;
    size_t capacity;
} String;

char* read_file(const char* file_path);

const char* strip_left(const char* str, size_t str_len);
CStringArray split_by_delim(const char* str, size_t str_len, const char* delim);
CStringArray split_by_whitespace(const char* str, size_t str_len);


#ifdef UTILS_IMPL
char* read_file(const char* file_path)
{
    FILE* f = fopen(file_path, "rb");
    if (f == NULL) {
        fprintf(stderr, "ERROR: Could not open `%s` for reading: %s\n", file_path, strerror(errno));
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    int32_t capacity = ftell(f);
    if (capacity < 0) {
        fprintf(stderr, "ERROR: Could not ftell file: `%s`\n", file_path);
        fprintf(stderr, "Maybe it's a directory?\n", file_path);
        return NULL;
    }
    fseek(f, 0, SEEK_SET);

    char *buf = malloc(capacity * sizeof(char));
    assert(buf != NULL && "Not enough memory");

    fread(buf, 1, capacity, f);
    if (ferror(f)) {
        fprintf(stderr, "ERROR: Could not read `%s`: %s\n", file_path, strerror(errno));
        free(f);
        return NULL;
    }
    free(f);
    return buf;
}

const char* strip_left(const char* str, size_t str_len)
{
    for (size_t i = 0; i < str_len && isspace(*str); ++i) {
        str++;
    }
    return str;
}


CStringArray split_by_delim(const char* str, size_t str_len, const char* delim)
{
    CStringArray result = {0};
    result.data = NULL;
    result.capacity = SPLIT_CAPACITY;
    result.length = 0;

    result.data = malloc(result.capacity * sizeof(char*));
    assert(result.data != NULL && "Not enough memory");
    memset(result.data, 0, sizeof(char*) * result.capacity);

    char* tmp;
    char* token;
    char* rest = (char*)str;

    while ((token = strtok_r(rest, delim, &rest))) {
           if (result.length >= result.capacity) {
               result.capacity = result.capacity * 2;
               result.data = realloc(result.data, result.capacity * sizeof(char*));
               assert(result.data && "Buy more ram");
           }
        result.data[result.length++] = token;
    }
    return result;
}

CStringArray split_by_whitespace(const char* str, size_t str_len)
{
    return split_by_delim(str, str_len, " \t\n\0");
}
#endif // UTILS_IMPL

#endif // UTILS_H_
