# HashTable in C

## Usage (word frequency example)
```
$ make
$ ./word_freq ./shakespeare.txt
`the`: 27483
`and`: 25991
`i`: 19540
`to`: 18656
`of`: 17952
`a`: 14365
`my`: 12455
`in`: 10660
`you`: 10597
`that`: 10473
$ make measure-time
time ./word_freq ./shakespeare.txt 1>/dev/null

real	0m0.112s
user	0m0.103s
sys	0m0.009s
--------------------------------------------
time ./word_freq.py ./shakespeare.txt 1>/dev/null

real	0m0.427s
user	0m0.338s
sys	0m0.089s

```

## Example
```
#include <stdio.h>

#define HASHMAP_IMPL
#include "hashmap.h"

int main(void)
{
    // init & allocate hashmap
    HashMap fruits = hashmap_init();

    // add element to hashmap
    hashmap_add(&fruits, "apples", 12);

    // get element from hashmap
    Hash_Result apples = hashmap_get(&fruits, "apples");
    if (apples.hasValue) {
        printf("apples: %ld\n", apples.value);
    }

    hashmap_add(&fruits, "bananas", 3);
    hashmap_add(&fruits, "pears", 5);

    // pop element from hashmap
    hashmap_pop(&fruits, "apples");

    // pop element with checking
    if (!hashmap_pop(&fruits, "cucumbers").hasValue) {
        printf("err: no `cucumbers` in `fruits`\n");
    }

    // print hashmap elements
    printf("Fruits:\n");
    for (int i = 0; i < fruits.length; ++i) {
        const char *key = fruits.keys[i];
        printf("  `%s` -> %ld \n", key, hashmap_get(&fruits, key).value);
    }

    // free allocations
    hashmap_free(&fruits);
    return 0;
}
```

