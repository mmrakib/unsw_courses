#!/bin/dash

for IMAGE in "$@"
do
    display $IMAGE
    echo -n "Address to email this image to? "
    read EMAIL
    if ! [ -z "$EMAIL" ];
    then
        echo -n "Message to accompany image? "
        read MESSAGE
        echo "$MESSAGE" | mutt -s "$IMAGE" -e 'set copy=no' -a $IMAGE -- $EMAIL
        echo "$IMAGE sent to $EMAIL"
    else
        echo "No email sent"
    fi
done
