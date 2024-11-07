#!/bin/dash

get_extension() {
    local first_line=$(head -n 1 < "$1")
    if echo "$first_line" | grep -Eq "perl"; then
        extended_filename="$1.pl"
    elif echo "$first_line" | grep -Eq "python" "$first_line"; then
        extended_filename="$1.py"
    elif echo "$first_line" | grep -Eq "sh" "$first_line"; then
        extended_filename="$1.sh"
    fi
}

for arg in "$@"; do
    first_line=$(head -n 1 < "$arg")
    if echo "$arg" | grep -Eq "."; then
        echo "$arg already has an extension"
    elif echo "$first_line" | grep -Evq "#!"; then
        echo "$arg does not have a #! line"
    elif echo "$first_line" | grep -Evq "(perl|python|sh)"; then
        echo "$arg no extension for #! line"
    else
        get_extension "$arg"
        if [ -e "$extended_filename" ]; then
            echo "$extended_filename already exists"
        else
            echo "mv $arg $extended_filename"
        fi
    fi
done
