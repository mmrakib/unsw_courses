#!/usr/bin/python3

import sys, re

surnames = set()
for line in sys.stdin:
    m = re.search(r'M$', line)
    if m:
        #print(line)
        name = re.sub(r'^(.*)\|(.*)\|(.*)\|.*$', r'\2', line)
        #print(name)
        surname = re.sub(r'(.*),.*', r'\1', name)
        surnames.add(surname)

for surname in sorted(surnames):
    print(surname, end='')
