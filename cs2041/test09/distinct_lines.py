#!/usr/bin/python3

import sys

n = int(sys.argv[1])
lines = set()
lc = 0

for line in sys.stdin:
    line = line.lower().strip()
    line = ' '.join(line.split())
    lines.add(line)
    lc += 1
    if len(lines) >= n:
        print(f'{len(lines)} distinct lines seen after {lc} lines read.')
        exit(0)
print(f'End of input reached after {lc} lines read - {n} different lines not seen.')
