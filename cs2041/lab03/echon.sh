#!/bin/dash

if [ $# -ne 2 ]
then
    echo "Usage: ./echon.sh <number of lines> <string>"
    exit 1
fi

case "${1#[+-]}" in
    ''|*[!0-9]*)
        echo "./echon.sh: argument 1 must be a non-negative integer"
        exit 1
        ;;
    *)
        :
        ;;
esac

N=$1
LINE=$2

if [ $N -lt 0 ]
then
    echo "./echon.sh: argument 1 must be a non-negative integer"
    exit 1
fi

I=0
while [ $I -lt $N ]
do
    echo $LINE
    I=$(( $I + 1 ))
done
