#!/bin/dash

grep -E "M$" - | cut -d '|' -f 3 | sed -E "s/(.*),.*/\1/" | sort | uniq | cat
