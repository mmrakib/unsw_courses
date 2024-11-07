#!/bin/dash

sorted=$(sort -n <$1)
counter=$(echo $sorted | cut -d " " -f 1)

for number in `echo $sorted`; do
    if [ $counter -ne $number ]; then
        echo $counter
        exit 0
    fi
    counter=$(($counter + 1))
done
