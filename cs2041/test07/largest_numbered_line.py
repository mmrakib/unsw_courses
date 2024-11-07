#!/usr/bin/python3

import sys
import math

def IsNumber(x):
    try:
        n = float(x)
    except ValueError:
        return False
    return True

lines = []
for line in sys.stdin:
    lines.append(line.strip())

largest_num = -math.inf
largest_line = ''

for line in lines:
    numbers = [float(x) for x in line.split() if IsNumber(x)]
    numbers.sort()
    if (len(numbers) != 0):
        if (largest_num < numbers[-1]):
            largest_num = numbers[-1]
            largest_line = line

print(largest_line)
