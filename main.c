#include <stdio.h>
#include <ctype.h>

#define UTILS_IMPL
#include "utils.h"
#define HASHMAP_IMPL
#include "hashmap.h"

int hash_pairs_compare(const void* ptr1, const void* ptr2)
{
    return ((Hash_Pair*)ptr1)->value < ((Hash_Pair*)ptr2)->value;
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
    HashMap hm = hashmap_init_cap(65536*2);

    CStringArray res = split_by_whitespace(content, strlen(content));

    for (size_t i = 0; i < res.length; ++i) {
        char* p = res.data[i];
        for(; *p; p++) *p = tolower(*p);
        hashmap_add(&hm, res.data[i], 1+hashmap_get(&hm, res.data[i]).value);
    }

    Hash_Pair* pairs = hashmap_to_pairs(&hm);
    size_t pairs_len = hm.length;

    qsort(pairs, pairs_len, sizeof(Hash_Pair), hash_pairs_compare);

    for (size_t i = 0; i < 10; ++i) {
        printf("`%s`: %ld\n", pairs[i].key, pairs[i].value);
    }

    printf("\n");
    printf("cap: %lu\n", hm.capacity);
    printf("len: %lu\n", hm.length);

    free(res.data);
    free(content);
    free(pairs);
    hashmap_free(&hm);
    return 0;
}

