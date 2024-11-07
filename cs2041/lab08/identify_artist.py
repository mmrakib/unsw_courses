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
    print("Usage: python3 identify_artist.py <file1> <file2> <file3> ...")
    exit(1)

args = sys.argv[1:]

for filename in args:
    data = []
    f = open(filename)
    song_words = re.split('[^a-zA-Z]', f.read())
    song_words = list(filter(lambda x: x != '', song_words))
    song_words = [x.lower() for x in song_words]

    for lyric_filename in os.listdir('lyrics'):
        artist = os.path.splitext(lyric_filename)[0].replace('_', ' ')
        path = os.path.join('lyrics', lyric_filename)

        with open(path) as lf:
            lyric_lines = lf.readlines()
            log_prob = calcLogProb(lyric_lines, song_words)
            data.append({'artist': artist, 'log_prob': log_prob})

    lowest = {'artist': 'None', 'log_prob': -math.inf}
    for d in data:
        if (d['log_prob'] > lowest['log_prob']):
            lowest = d

    print(f'{filename} most resembles the work of {lowest["artist"]} (log-probability={lowest["log_prob"]:.1f})')
    f.close()
