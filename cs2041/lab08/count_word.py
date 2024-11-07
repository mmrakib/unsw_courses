#!/usr/bin/python3

import sys
import re

if (len(sys.argv) != 2):
    print("Usage: python3 count_word.py <word> < <input file>")
    exit(1)

word = str(sys.argv[1])
totalcount = 0

for line in sys.stdin:
    a = re.split('[^a-zA-Z]', line)
    a = list(filter(lambda x: x != '', a))
    a = [x.lower() for x in a]
    count = a.count(word)
    totalcount += count

print(f'{word} occurred {totalcount} times')
