#!/bin/dash

regex=$1
filename=$2

years=$(grep -E "^$regex\$" < "$filename" | cut -d '|' -f 2 | sort -n | uniq)

first=$(echo "$years" | cut -d " " -f 1)
last=$first
for number in $years; do
    if [ "$number" -ne $((last + 1)) ] && [ "$number" -ne "$first" ]; then
        counter=$((last + 1))
        while [ $counter -lt "$number" ]; do
            echo $counter
            counter=$((counter + 1))
        done
    fi
    last=$number
done
