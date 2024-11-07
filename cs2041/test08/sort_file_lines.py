#!/usr/bin/python3

import sys

if (len(sys.argv) != 2):
    print('Usage: python3 sort_file_lines.py <file>')
    exit(1)

filename = sys.argv[1]

with open(filename) as f:
    counter = 0
    lines = []
    for line in f:
        lines.append((line, counter))
        counter += 1

    sorted_lines = sorted(lines, key=lambda x: (len(x[0]), x[1]))
    
    for line, _ in sorted_lines:
        print(line, end='')

    #lines = f.readlines()
    #lines = sorted(lines, key=lambda x: len(x))
    #for line in lines:
    #    print(line, end='')
