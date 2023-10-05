#!/bin/bash
FIRST_FILE=$1
SECOND_FILE=$2

DATA_FIRST=$(grep -Eo "Result:\s[[:print:]]*" "$FIRST_FILE")
DATA_FIRST=${DATA_FIRST:8}
DATA_SECOND=$(grep -Eo "Result:\s[[:print:]]*" "$SECOND_FILE")
DATA_SECOND=${DATA_SECOND:8}

if [[ "$DATA_FIRST" == "$DATA_SECOND" ]]; then
    exit 0
else
    exit 1
fi

