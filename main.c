
#include <stdio.h>
#include <ctype.h>

#define UTILS_IMPL
#include "utils.h"
#define HASHMAP_IMPL
#include "hashmap.h"

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
    for (size_t i = 0; i < hm.length; ++i) {
        printf("`%s`: %d\n", hm.keys[i], hashmap_get(&hm, hm.keys[i]).value);
    }

    free(res.data);
    free(content);
    // TODO: free_hashmap(hm);
    return 0;
}
