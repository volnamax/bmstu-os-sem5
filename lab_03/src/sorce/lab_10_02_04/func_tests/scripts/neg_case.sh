#!/bin/bash

neg_in=$1 # имя файла для подмены потока входа
ARGS=$2

name_exe="./app.exe"
#temp="temp.temp" # для вывода программы 
valgrind_temp="out/valgrind.temp" # для обработки ошибки с валгриндом
#eval "echo > $temp"
valgrind_all="out/valgrind_all.temp" # сохранение ошибок с валгринда

error_memory=0
temp="func_tests/data/temp.temp" # для вывода программы 

eval "> $temp"
# если есть глобвльная переменная USE_VALGRIND, то программа запускается под его управлением
if [[ -n "$USE_VALGRIND" ]]; then
    eval "valgrind --log-file=$valgrind_temp --tool=memcheck --leak-check=full  -q --track-origins=yes $name_exe < $neg_in > $temp "
    echo $?
    
    eval "echo TEST_VALGRIND $neg_in >> $valgrind_all"
    
    if [[ -s $valgrind_temp  ]]; then
        string_error=$(<$valgrind_temp) # так как я использую wsl у меня валгринд ругается постоянно на одну ошибку, по этому я ее исключаю
        string_error=${string_error:9} #обрезаем номер ошибки 
        string_admissible_err=" error calling PR_SET_PTRACER, vgdb might block"
        eval "cat $valgrind_temp >> $valgrind_all"
        eval "echo ============================================= >> $valgrind_all" 

        if [[ "$string_error" == "$string_admissible_err" ]]; then
            error_memory=0
        else
            error_memory=1
        fi
    fi
  else 

    eval "$name_exe < $neg_in > $temp "
    value_return=$?
    echo $value_return
fi

if [[ $value_return == 0 ]]; then
    if [[ $error_memory == 0 ]]; then
        exit 2 #тесты не прошли, но память прошла
    else
        exit 3 #тесты не прошли проверку, память тоже
    fi
else
    if [[ $error_memory == 0 ]]; then
        exit 0 #Все ок
    else
        exit 1 #тесты прошли проверку, память нет тоже
    fi
fi
