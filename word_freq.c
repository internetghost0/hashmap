#include <stdio.h>
#include <ctype.h>

#define UTILS_IMPL
#include "utils.h"
#define HASHMAP_IMPL
#include "hashmap.h"

int hashmap_pairs_compare(const void* ptr1, const void* ptr2)
{
    return ((HM_Pair*)ptr1)->value < ((HM_Pair*)ptr2)->value;
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
    if (!content) {
        return -1;
    }

    CStringArray res = split_by_whitespace(content);
    if (res.length == 0) {
        fprintf(stderr, "ERROR: empty file\n");
        free(content);
        free(res.data);
        return -1;
    }

    HashMap* hm = hashmap_init_cap(65536*2);
    //HashMap* hm = hashmap_init();

    for (size_t i = 0; i < res.length; ++i) {
        char* p = res.data[i];
        for(; *p; p++) *p = tolower(*p);
        hashmap_add(hm, res.data[i], 1+hashmap_get(hm, res.data[i]).value);
    }

    HM_Pair* pairs = hashmap_to_pairs(hm);
    size_t pairs_len = hm->length;

    qsort(pairs, pairs_len, sizeof(HM_Pair), hashmap_pairs_compare);

    for (size_t i = 0; i < 10 && i < pairs_len; ++i) {
        printf("`%s`: %ld\n", pairs[i].key, pairs[i].value);
    }

    free(pairs);
    free(res.data);
    hashmap_free(hm);
    free(content);
    return 0;
}

