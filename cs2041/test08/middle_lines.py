#!/usr/bin/python3

import sys
import math

if (len(sys.argv) != 2):
    print('Usage: python3 middle_lines.py <file>')
    exit(1)

filename = sys.argv[1]

with open(filename) as f:
    lines = f.readlines()
    num_lines = len(lines)

    if (num_lines <= 0):
        exit(0)

    if (num_lines % 2 == 0):
        lower = math.floor((num_lines / 2) - 1)
        upper = math.floor(num_lines / 2)
        print(lines[lower], end='')
        print(lines[upper], end='')
    else:
        index = math.floor(num_lines / 2)
        print(lines[index], end='')
