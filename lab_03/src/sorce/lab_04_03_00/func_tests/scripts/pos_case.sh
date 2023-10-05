#!/bin/bash

pos_in=$1   # имя файла для подмены потока входа	
pos_out=$2  # имя файла с истиными значениями вывода
FILE_ARGS=""
# если есть позицных аргумент с именем файла с ключами, то считываем его в переменную
error_memory=0

if [[ $# -gt 2 ]]; then
    FILE_ARGS=$3
    ARGS=$(cat "$FILE_ARGS")
fi

# если есть глобальная переменная USE_VALGRIND, то программа запускается под его управлением
if [[ -n "$USE_VALGRIND" ]]; then
    eval "valgrind --log-file=valgrind_pos.temp -q ../../app.exe < $pos_in > all_out.txt $ARGS"
    
    if [[ -s "valgrind_pos.temp" ]]; then
        error_memory=1
    fi
else
    eval "../../app.exe < $pos_in > all_out.txt $ARGS"
fi

if ./comparator.sh all_out.txt "$pos_out"; then
    if [[ $error_memory == 0 ]]; then
        exit 0
    else
        exit 1 #тесты прошли проверку, память нет
    fi
else
    if [[ $error_memory == 0 ]]; then
        exit 2 #тесты не прошли, но память прошла
    else
        exit 3 #тесты не прошли проверку, память тоже
    fi
fi
