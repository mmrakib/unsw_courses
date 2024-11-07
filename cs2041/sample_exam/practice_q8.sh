#!/bin/dash

for filename in $@; do
    byte_count=$(wc -c "$filename")
    echo "$byte_count" 
done
