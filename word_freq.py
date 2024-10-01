#!/bin/env python3

import sys

if __name__ == '__main__':

    if len(sys.argv) != 2:
        print('Error: No input file provided')
        exit(f'Usage: {sys.argv[0]} <input>')

    fn = sys.argv[1]

    hashmap = {}
    pairs = []
    data = open(fn, 'r').read().split()

    for word in data:
        word = word.lower()
        hashmap[word] = hashmap.get(word, 0) + 1

    for key, val in hashmap.items():
        pairs.append((key, val))

    pairs.sort(key=lambda x: x[1], reverse=True)

    for i, pair in enumerate(pairs):
        if (i >= 10): break
        print(f'`{pair[0]}`: {pair[1]}')
