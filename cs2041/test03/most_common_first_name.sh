#!/bin/dash

cat /dev/stdin | cut -d'|' -f3 | cut -d',' -f2 | awk '{$1=$1};1' | sed -E 's/ .*//' | sort | uniq -c | sort -rn | head -1 | awk '{$1=$1};1' | cut -d' ' -f2
