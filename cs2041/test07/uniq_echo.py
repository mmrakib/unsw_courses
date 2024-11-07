#!/usr/bin/python3

import sys

if (len(sys.argv) < 2):
    print('Usage: python3 uniq_echo.py <word1> <word2> ...')
    exit(1)

words = sys.argv[1:]
printed = []

for word in words:
    if (word not in printed):
        printed.append(word)
        print(word, end=' ')

print('')
