#!/bin/dash

echo -n "Small files: "
for FILE in *;
do
    SIZE=$(wc -l $FILE | tr ' ' '\n' | head -1)
    if [ "$SIZE" -lt 10 ]
    then
        echo -n $FILE
        echo -n ' '
    else
        echo -n ''
    fi
done
echo ''

echo -n "Medium-sized files: "
for FILE in *;
do
    SIZE=$(wc -l $FILE | tr ' ' '\n' | head -1)
    if [ "$SIZE" -ge 10 ] && [ "$SIZE" -lt 100 ]
    then
        echo -n $FILE
        echo -n ' '
    else
        echo -n ''
    fi
done
echo ''

echo -n "Large files: "
for FILE in *;
do
    SIZE=$(wc -l $FILE | tr ' ' '\n' | head -1)
    if [ "$SIZE" -ge 100 ]
    then
        echo -n $FILE
        echo -n ' '
    else
        echo -n ''
    fi
done
echo ''
