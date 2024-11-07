#!/bin/dash

for IMAGE in "$@"
do 
    MONTH=$(ls -l "$IMAGE" | cut -d' ' -f 6)
    DAY=$(ls -l "$IMAGE" | cut -d' ' -f 7)
    TIME=$(ls -l "$IMAGE" | cut -d' ' -f 8)

    convert -gravity south -pointsize 36 -draw "text 0,10 '$MONTH $DAY $TIME'" $IMAGE $IMAGE
done
