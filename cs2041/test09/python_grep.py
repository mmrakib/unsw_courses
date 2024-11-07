#!/usr/bin/python3

import sys
import re

regex = sys.argv[1]
filename = sys.argv[2]

with open(filename) as f:
    lines = f.readlines()
    for line in lines:
        m = re.search(regex, line)
        if m:
            print(line, end='')
