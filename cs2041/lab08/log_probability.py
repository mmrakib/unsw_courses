#!/usr/bin/python3

import sys
import os
import re
import math

def totalWords(lines):
    totalwc = 0
    for line in lines:
        a = re.split('[^a-zA-Z]', line)
        a = list(filter(lambda x: x != '', a))
        wc = len(a)
        totalwc += wc
    return totalwc

def countWord(lines, word):
    totalcount = 0
    for line in lines:
        a = re.split('[^a-zA-Z]', line)
        a = list(filter(lambda x: x != '', a))
        a = [x.lower() for x in a]
        count = a.count(word)
        totalcount += count
    return totalcount

def calcLogProb(lines, words):
    total_words = totalWords(lines)
    log_prob = 0
    for word in words:
        count = countWord(lines, word) + 1
        freq = count / total_words
        log_prob += math.log(freq)
    return log_prob

if (len(sys.argv) < 2):
    print("Usage: python3 frequency.py <word1> <word2> <word3> ...")
    exit(1)

words = sys.argv[1:]
results = []

for filename in os.listdir('lyrics'):
    artist = os.path.splitext(filename)[0].replace('_', ' ')
    path = os.path.join('lyrics', filename)

    res = {}
    res['artist'] = artist

    with open(path) as f:
        lines = f.readlines()
        res['log_prob'] = calcLogProb(lines, words)

    results.append(res)

results = sorted(results, key=lambda x: x['artist'])

for r in results:
    print(f"{r['log_prob']:10.5f} {r['artist']}")
