#!/bin/bash

DATA_FIRST=$(<"$1") #cчитали фалй в переменую 
DATA_SECOND=$(<"$2")
# сравниваем строки
#diff -y $1 $2

if [[ "$DATA_FIRST" == "$DATA_SECOND" ]]; then
    exit 0
else
    exit 1
fi