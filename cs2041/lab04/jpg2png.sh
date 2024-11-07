#!/bin/dash

for FILE in *;
do
    if [ "${FILE#*.}" = "jpg" ];
    then
        if ! [ -f "${FILE%.*}.png" ];
        then
            convert "$FILE" "${FILE%.*}.png"
            rm "$FILE"
        else
            echo "${FILE%.*}.png already exists"
        fi
    fi
done
