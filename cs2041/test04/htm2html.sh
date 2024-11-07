#!/bin/dash

for FILE in *;
do
    SUFFIX=$(echo "$FILE" | sed 's/.*\.//')
    if [ "$SUFFIX" = "htm" ]
    then
        NEWFILE=$(echo "$FILE" | sed 's/\.htm/\.html/')
        if [ ! -e "$NEWFILE" ]
        then
            mv "$FILE" "$NEWFILE"
        else
            echo "$NEWFILE exists"
            exit 1
        fi
    fi
done
