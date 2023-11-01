#ifndef UTILS_H_
#define UTILS_H_
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <assert.h>


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
char* read_file_cap(const char* file_path, size_t capacity);

const char* strip_left(const char* str, size_t str_len);
CStringArray split_by_space(const char* str, size_t str_len);
CStringArray split_by_delim(const char* str, size_t str_len, char delim);



#ifdef UTILS_IMPL
char* read_file_cap(const char* file_path, size_t capacity)
{
    if (capacity == 0) {
        capacity = 32*1024;
    }
    size_t length = 0;
    char *buf = malloc(capacity * sizeof(char));
    assert(buf != NULL && "Not enough memory");
    FILE* f = fopen(file_path, "rb");
    if (f == NULL) {
        fprintf(stderr, "Could not open %s for reading: %s", file_path, strerror(errno));
        return NULL;
    }
    size_t n = fread(buf, 1, capacity, f);
    while (n > 0) {
        // double buffer size
        if (length + n >= capacity) {
            capacity = capacity * 2 + length;
            buf = realloc(buf, capacity * sizeof(char));
            assert(buf != NULL && "Not enough memory");
        }
        length += n;
        n = fread(buf + length, 1, capacity - length, f);
    }
    if (ferror(f)) {
        fprintf(stderr, "Could not read %s: %s\n", file_path, strerror(errno));
        return NULL;
    }
    free(f);
    return buf;
}
char* read_file(const char* file_path)
{
    return read_file_cap(file_path, 0);
}

const char* strip_left(const char* str, size_t str_len)
{
    for (size_t i = 0; i < str_len && isspace(*str); ++i) {
        str++;
    }
    return str;
}

CStringArray split_by_space(const char* str, size_t str_len)
{
    CStringArray result = {0};
    result.data = NULL;
    result.capacity = 1024;
    result.length = 0;

    result.data = malloc(result.capacity * sizeof(char*));
    assert(result.data != NULL && "Not enough memory");
    memset(result.data, 0, sizeof(char*) * result.capacity);
    size_t begin = 0;
    for (size_t i = 1; i < str_len; ++i) {
        if (isspace(str[i - 1]) && !isspace(str[i])) {
            begin = i;
        }
        else if (isspace(str[i]) && !isspace(str[i-1]) || i == str_len - 1) {
           size_t word_len = i - begin;
            if (i == str_len - 1) {
                word_len++;
            }
           // double buffer size
           if (word_len + result.length >= result.capacity) {
               result.capacity = result.capacity * 2 + word_len;
               result.data = realloc(result.data, result.capacity * sizeof(char*));
           }
           char *tmp = malloc(1 + word_len * sizeof(char));
           memcpy(tmp, str + begin, word_len * sizeof(char));
           result.data[result.length++] = tmp;
       }
    }
    return result;
}
CStringArray split_by_delim(const char* str, size_t str_len, char delim)
{
    CStringArray result = {0};
    result.data = NULL;
    result.capacity = 1024;
    result.length = 0;
    result.data = malloc(result.capacity * sizeof(char*));
    assert(result.data != NULL && "Not enough memory");
    memset(result.data, 0, sizeof(char*) * result.capacity);
    size_t begin = 0;
    for (size_t i = 0; i < str_len; ++i) {
        if (str[i] == delim || i == str_len - 1) {
           size_t word_len = i - begin;
           if (i == str_len - 1) {
               word_len++;
           }
           // double buffer size
           if (word_len + result.length >= result.capacity) {
               result.capacity = result.capacity * 2 + word_len;
               result.data = realloc(result.data, result.capacity * sizeof(char*));
           }
           char *tmp = malloc(1 + word_len * sizeof(char));
           memcpy(tmp, str + begin, word_len * sizeof(char));
           result.data[result.length++] = tmp;
           begin = i + 1;
       }
    }
    return result;
}
#endif // UTILS_IMPL

#endif // UTILS_H_
