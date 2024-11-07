#!/bin/dash

cat /dev/stdin | sort -t'|' -k 2 | cut -d'|' -f2 | uniq -c | grep -E '2 .*' | awk '{$1=$1};1' | cut -d' ' -f2
