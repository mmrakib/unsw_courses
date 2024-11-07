#!/bin/dash

for FILE in "$@";
do
    while read -r LINE;
    do
        IS_INCLUDE=$(echo "$LINE" | grep -E "^#include \"(.*)\"")
        if [ -n "$IS_INCLUDE" ]
        then
            INCLUDE_FILE=$(echo "$LINE" | sed -E "s/^#include \"(.*)\"/\1/")
            if [ ! -e "$INCLUDE_FILE" ]
            then
                echo "$INCLUDE_FILE included into $FILE does not exist"
            fi
        fi
    done < "$FILE"
done
