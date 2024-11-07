#!/usr/bin/python3

import sys

if (len(sys.argv) < 2):
    print('Usage: python3 snap_n.py <times until snap>')
    exit(1)

words = []
n = int(sys.argv[1])

while True:
    try:
        word = input()
    except EOFError:
        break
    if any(w['word'] == word for w in words):
        w = next(filter(lambda x: x['word'] == word, words))
        w['count'] += 1

        if (w['count'] >= n):
            print(f'Snap: {w["word"]}')
            break
    else:
        w = {'word': word, 'count': 1}
        words.append(w)
