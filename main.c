#include <stdio.h>
#include <ctype.h>

#define UTILS_IMPL
#include "utils.h"
#define HASHMAP_IMPL
#include "hashmap.h"

int main2(void)
{
    HashMap hm = hashmap_init_cap(1);
    hashmap_add(&hm, "a", 1);
    hashmap_add(&hm, "a", 1);
    hashmap_add(&hm, "a", 1);
    hashmap_add(&hm, "a", 1);
    hashmap_add(&hm, "a", 1);
    hashmap_add(&hm, "a", 1);
    hashmap_add(&hm, "a", 1);
    hashmap_add(&hm, "a", 1);
    hashmap_add(&hm, "a2", 1);
    hashmap_add(&hm, "a2", 1);
    hashmap_add(&hm, "a2", 1);
    hashmap_add(&hm, "a2", 1);
    hashmap_add(&hm, "a2", 1);
    hashmap_add(&hm, "a2", 1);
    hashmap_add(&hm, "a2", 1);
    hashmap_free(&hm);
    return 0;
}

int main1(void)
{
    HashMap hm = hashmap_init_cap(1);
    hashmap_add(&hm, "a", 1);
    hashmap_add(&hm, "a", 1);
    hashmap_add(&hm, "a", 1);
    hashmap_add(&hm, "a", 1);
    hashmap_add(&hm, "a2", 2);
    hashmap_add(&hm, "a3", 3);
    hashmap_add(&hm, "a4", 4);
    hashmap_add(&hm, "a5", 5);
    hashmap_add(&hm, "a6", 6);
    hashmap_add(&hm, "a6", 6);
    hashmap_add(&hm, "a7", 7);
    hashmap_add(&hm, "a8", 8);
    hashmap_add(&hm, "a9", 9);
    hashmap_add(&hm, "a9", 9);
    hashmap_add(&hm, "a10", 10);
    hashmap_add(&hm, "a11", 11);
    printf("%s\n",  hashmap_pop(&hm, "a").value == 1  ? "true" : "false");
    printf("%s\n", hashmap_get(&hm, "a2").value == 2  ? "true" : "false");
    printf("%s\n", hashmap_get(&hm, "a3").value == 3  ? "true" : "false");
    printf("%s\n", hashmap_get(&hm, "a4").value == 4  ? "true" : "false");
    printf("%s\n", hashmap_get(&hm, "a5").value == 5  ? "true" : "false");
    printf("%s\n", hashmap_get(&hm, "a6").value == 6  ? "true" : "false");
    printf("%s\n", hashmap_get(&hm, "a7").value == 7  ? "true" : "false");
    printf("%s\n", hashmap_get(&hm, "a8").value == 8  ? "true" : "false");
    printf("%s\n", hashmap_get(&hm, "a9").value == 9  ? "true" : "false");
    printf("%s\n", hashmap_get(&hm, "a10").value == 10? "true" : "false");
    printf("%s\n", hashmap_get(&hm, "a11").value == 11? "true" : "false");
    printf("len: %zu, cap: %zu\n", hm.length, hm.capacity);
    printf("a5: %d\n", hashmap_get(&hm, "a5").value);
    printf("[ ");
    for (size_t i = 0; i < hm.length; ++i)
    {
        printf("`%s` ", hm.keys[i]);
    }
    printf("]\n");
    hashmap_free(&hm);
    return 0;
}

int main(int argc, char** argv) {
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
    Hash_Pair* pairs = hashmap_to_pairs(&hm);
    size_t pairs_len = hm.length;

    for (size_t i = 0; i < pairs_len-1; ++i) {
        for (size_t j = i+1; j < pairs_len; ++j) {
            if (pairs[i].value < pairs[j].value) {
                Hash_Pair t = pairs[i];
                pairs[i] = pairs[j];
                pairs[j] = t;
            }
        }
    }
    for (size_t i = 0; i < pairs_len; ++i) {
        printf("`%s`: %d\n", pairs[i].key, pairs[i].value);
    }

    free(res.data);
    free(content);
    free(pairs);
    hashmap_free(&hm);
    return 0;
}
