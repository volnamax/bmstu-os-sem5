#!/bin/bash
pos_in=$1   # имя файла для подмены потока входа	
pos_out=$2  # имя файла с истиными значениями вывода
ARGS=$3     # имя файла с ключами
error_memory=0
name_exe="./app.exe"
valgrind_temp="out/valgrind.temp" # для обработки ошибки с валгриндом
valgrind_all="out/valgrind_all.temp" # сохранение ошибок с валгринда

temp="func_tests/data/temp.temp" # для вывода программы 
value_return=0
eval "> $temp"


if [[ -n "$USE_VALGRIND" ]]; then
    eval "echo > $valgrind_temp"
    eval "valgrind --log-file=$valgrind_temp  --tool=memcheck --leak-check=full --show-leak-kinds=all  -q --track-origins=yes  $name_exe < $pos_in > $temp "
    value_return=$?
    eval "echo TEST_VALGRIND $pos_out >> $valgrind_all"

    if [[ -s $valgrind_temp ]]; then #если файл с валгриндом не пуст, то ошибка
        string_error=$(<$valgrind_temp) # так как я использую wsl у меня валгринд ругается постоянно на одну ошибку, по этому я ее исключаю
        string_error=${string_error:9} #обрезаем номер ошибки
        eval "cat $valgrind_temp >> $valgrind_all"
        eval "echo ============================================= >> $valgrind_all" 
        string_admissible_err=" error calling PR_SET_PTRACER, vgdb might block"
        if [[ "$string_error" == "$string_admissible_err" ]]; then
            error_memory=0
        else
            error_memory=1
        fi
    fi
else
    eval "$name_exe < $pos_in > $temp"
    
fi

#это необходимо если у нас не было вывода в темп файл, то передаем в компаратор измененный входной файл

if ./func_tests/scripts/comparator.sh "$temp" "$pos_out"; then
    if [[ $error_memory == 0 ]]; then
        exit 0
    else
        echo $value_return
        exit 1 #тесты прошли проверку, память нет
    fi
else
    if [[ $error_memory == 0 ]]; then
        echo $value_return

        exit 2 #тесты не прошли, но память прошла
    else
        echo $value_return

        exit 3 #тесты не прошли проверку, память тоже
    fi
fi