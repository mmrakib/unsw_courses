#!/usr/bin/python3

import sys
import os
import re

if (len(sys.argv) != 2):
    print("Usage: python3 frequency.py <word>")
    exit(1)

word = str(sys.argv[1])
results = []

for filename in os.listdir('lyrics'):
    artist = os.path.splitext(filename)[0].replace('_', ' ')
    path = os.path.join('lyrics', filename)

    with open(path) as f:
        lines = f.readlines()
        words = 0
        count = 0

        for line in lines:
            a = re.split('[^a-zA-Z]', line)
            a = list(filter(lambda x: x != '', a))
            a = [x.lower() for x in a]
            
            words += len(a)
            count += a.count(word)
        
        freq = count / words
        result = {'artist': artist, 'words': words, 'count': count, 'freq': freq}
        results.append(result)

results = sorted(results, key=lambda x: x['artist'])
for r in results:
    print(f"{r['count']:4}/{r['words']:6} = {r['freq']:.9f} {r['artist']}")
