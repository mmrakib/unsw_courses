#!/usr/bin/python3

import sys
import operator

vowels = 'aeiouAEIOU'

def IsVowel(letter):
    if (operator.countOf(vowels, letter) > 0):
        return True
    return False

def ContainsConsecutiveVowels(word):
    cons = 0
    for letter in word:
        if (IsVowel(letter)):
            cons += 1
        else:
            cons = 0
        if (cons >= 3):
            return True
    return False

words = sys.argv[1:]

for word in words:
    if (ContainsConsecutiveVowels(word)):
        print(f'{word}', end=' ')
print('')
