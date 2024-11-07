#!/usr/bin/python3

import sys
import statistics
import math

nums = []
for arg in sys.argv[1:]:
    if '.' in arg:
        n = float(arg)
    else:
        n = int(arg)
    nums.append(n)

print(f'count={len(nums)}')
print(f'unique={len(set(nums))}')
print(f'minimum={min(nums)}')
print(f'maximum={max(nums)}')
print(f'mean={statistics.mean(nums)}')
print(f'median={statistics.median(nums)}')
print(f'mode={statistics.mode(nums)}')
print(f'sum={sum(nums)}')
print(f'product={math.prod(nums)}')
