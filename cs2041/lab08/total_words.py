#!/usr/bin/python3

from sys import stdin
import re

totalwc = 0

for line in stdin:
    a = re.split('[^a-zA-Z]', line)
    a = list(filter(lambda x: x != '', a))
    wc = len(a)
    totalwc += wc

print(f'{totalwc} words')
