#!/bin/dash

if [ $# -ne 3 ]
then
    echo "Usage: ./create_integers_file.sh <start> <finish> <filename>"
    exit 1
fi

case "${1#[+-]}" in 
    ''|*[!0-9]*)
        echo "./create_integers_file.sh: argument 1 must be an integer"
        exit 1
        ;;
    *)
        :
        ;;
esac

case "${2#[+-]}" in 
    ''|*[!0-9]*)
        echo "./create_integers_file.sh: argument 2 must be an integer"
        exit 1
        ;;
    *)
        :
        ;;
esac

if [ ! -e "$3" ]
then
    touch "$3"
else 
    rm "$3"
fi

I=$1
N=$(( $2 + 1 ))
while [ $I -lt $N ]
do
    echo "$I" >> $3
    I=$(( $I + 1 ))
done
