#!/bin/dash

cat /dev/stdin | cut -d'|' -f2,3 | sort | uniq | cut -d'|' -f2 | sed 's/ //' | cut -d',' -f 2 | sed -E 's/ .*//' | sort
