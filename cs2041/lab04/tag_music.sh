#!/bin/dash

for DIR in "$@"
do
    ALBUM=$(echo "${DIR#*/}" | sed 's/\///' | sed "s/'//")
    YEAR=$(echo $ALBUM | sed -E 's/(.*), (.*)/\2/')

    for SONG in "$DIR"/*
    do
        TRACK=$(echo $SONG | sed 's/\.[^.]*$//' | sed -E 's/(.*)\///' | sed -E 's/(.*) - (.*) - (.*)/\1/')
        TITLE=$(echo $SONG | sed 's/\.[^.]*$//' | sed -E 's/(.*)\///' | sed -E 's/(.*) - (.*) - (.*)/\2/')
        ARTIST=$(echo $SONG | sed 's/\.[^.]*$//' | sed -E 's/(.*)\///' | sed -E 's/(.*) - (.*) - (.*)/\3/')

        id3 -t "$TITLE" -T "$TRACK" -a "$ARTIST" -A "$ALBUM" -y "$YEAR" "$SONG" > /dev/null
    done
done
