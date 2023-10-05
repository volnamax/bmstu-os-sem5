#!/bin/bash

err_pos=0   # переменная, которая хранит кол-во проваленых позитивных тестов 
err_neg=0   # переменная, которая хранит кол-во проваленых негативных тестов 
iter=1 
num_file="0${iter}"     # переменная, для изменения номера в имени теста 

if [[ $1 != "-s" ]]; then   # ключ -s указывает на работу скрипта в тихом режиме 
    echo -e "\033[1mPositive test:\033[0m"
fi

# Пока находим два позитивных текстовых файла pos_num_in и pos_num_out с одинаковым num
# запускаем pos_case.sh с входными и истиными выходными данными
while [[ -f "../data/pos_${num_file}_in.txt" && -f "../data/pos_${num_file}_out.txt" ]]
do
  	pos_in="../data/pos_${num_file}_in.txt"
  	pos_out="../data/pos_${num_file}_out.txt"

    ./pos_case.sh "$pos_in" "$pos_out"
	value_return=$?
    # Если код возврата после работы скрипта pos_case = 0, то тест пройден
    if [[ $value_return == 0 ]]; then
        if [[ $1 != "-s" ]]; then
            echo -e "\033[42m Test ${iter}: PASS \033[0m"
        fi
    fi
    if [[ $value_return == 1 ]]; then
        if [[ $1 != "-s" ]]; then
            echo -e "\033[37;1;41m Test ${iter}: FAIL MEMORY \033[0m" 
            err_pos=$((err_pos + 1))      
        else
            err_pos=$((err_pos + 1))
        fi
    fi
    if [[ $value_return == 2 ]]; then
        if [[ $1 != "-s" ]]; then
            echo -e "\033[37;1;41m Test ${iter}: FAIL TEST (MEM ok)\033[0m" 
            err_pos=$((err_pos + 1))      
        else
            err_pos=$((err_pos + 1))
        fi
    fi
    if [[ $value_return == 3 ]]; then
        if [[ $1 != "-s" ]]; then
            echo -e "\033[37;1;41m Test ${iter}: FAIL TEST and MEM\033[0m" 
            err_pos=$((err_pos + 1))      
        else
            err_pos=$((err_pos + 1))
        fi
    fi
    
    
    # Это условие нужно для  номера в имени если он меньше 10
    iter=$((iter + 1))
    if [[ $iter -lt 10 ]]; then
        num_file="0${iter}"
    else
        num_file="$iter"
    fi
done

if [[ $1 != "-s" ]]; then
    echo -e "\n\033[1mNegative test:\033[0m"
fi

iter=1 
num_file="0${iter}"
# Тоже самое, что и для позитивных тестов

while [[ -f "../data/neg_${num_file}_in.txt" ]]
do
  	./neg_case.sh "../data/neg_${num_file}_in.txt"
	value_return=$?
    
    if [[ $value_return == 0 ]]; then
        if [[ $1 != "-s" ]]; then
            echo -e "\033[42m Test ${iter}: PASS \033[0m"
        fi
    fi
    if [[ $value_return == 1 ]]; then
        if [[ $1 != "-s" ]]; then
            echo -e "\033[37;1;41m Test ${iter}: FAIL MEMORY \033[0m" 
            err_pos=$((err_neg + 1))      
        else
            err_pos=$((err_neg + 1))
        fi
    fi
    if [[ $value_return == 2 ]]; then
        if [[ $1 != "-s" ]]; then
            echo -e "\033[37;1;41m Test ${iter}: FAIL TEST (MEM ok)\033[0m" 
            err_pos=$((err_neg + 1))      
        else
            err_pos=$((err_neg + 1))
        fi
    fi
    if [[ $value_return == 3 ]]; then
        if [[ $1 != "-s" ]]; then
            echo -e "\033[37;1;41m Test ${iter}: FAIL TEST and MEM\033[0m" 
            err_pos=$((err_neg + 1))      
        else
            err_pos=$((err_neg + 1))
        fi
    fi

    iter=$((iter + 1))
    if [[ $iter -lt 10 ]]; then
        num_file="0${iter}"
    else
        num_file="$iter"
    fi
done

if [[ $1 != "-s" ]]; then
    echo -e "\n\033[1mSum failed positive test = ${err_pos}\033[0m"
    echo -e "\033[1mSum failed negative test = ${err_neg}\033[0m"
fi

exit $((err_pos + err_neg)) 