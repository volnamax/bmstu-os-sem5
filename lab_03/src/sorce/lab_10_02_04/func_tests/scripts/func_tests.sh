#!/bin/bash
GREEN='\033[42m'
RED='\033[37;1;41m'
FORM='\033[0m'
H='\033[1m'
err_pos=0   # переменная, которая хранит кол-во проваленых позитивных тестов 
err_neg=0
iter=1 
num_file="0${iter}"     # переменная, для изменения номера в имени теста 
flag_args=0


if [[ $1 != "-s" ]]; then   # ключ -s указывает на работу скрипта в тихом режиме 
    echo -e "${H}Positive test:${FORM}"
fi

pos_in="func_tests/data/pos_${num_file}_in.txt"
pos_out="func_tests/data/pos_${num_file}_out.txt"
pos_args="func_tests/data/pos_${num_file}_args.txt"
infile="out/intemp.temp" 

# Пока находим два позитивных файла pos_num_in и pos_num_out с одинаковым num
# запускаем pos_case.sh с входными и истиными выходными данными и ключами 
while [[ -f $pos_in && -f $pos_out ]]
do
    eval "cp $pos_in $infile" #необходимо копирование входного файла, так как если прогрмма будет его измять, мы потеряем данные
  	pos_in=$infile

    if [[ -n "$ARGS" ]]; then 
        ARGS=$(<$pos_args)
        flag_args=1
    fi
    ./func_tests/scripts/pos_case.sh "$pos_in" "$pos_out" "$ARGS"
	value_return=$?
    # Если код возврата после работы скрипта pos_case = 0, то тест пройден
        if [[ $value_return == 0 ]]; then
        if [[ $1 != "-s" ]]; then
            echo -e "${GREEN} Test ${iter}: PASS ${FORM}"
        fi
    fi
    if [[ $value_return == 1 ]]; then
        if [[ $1 != "-s" ]]; then
            echo -e "${RED} Test ${iter}: FAIL MEMORY ${FORM}" 
            err_pos=$((err_pos + 1))      
        else
            err_pos=$((err_pos + 1))
        fi
    fi
    if [[ $value_return == 2 ]]; then
        if [[ $1 != "-s" ]]; then
            echo -e "${RED} Test ${iter}: FAIL TEST (MEM ok)${FORM}" 
            err_pos=$((err_pos + 1))      
        else
            err_pos=$((err_pos + 1))
        fi
    fi
    if [[ $value_return == 3 ]]; then
        if [[ $1 != "-s" ]]; then
            echo -e "${RED} Test ${iter}: FAIL TEST and MEM${FORM}" 
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

    pos_in="func_tests/data/pos_${num_file}_in.txt"
    pos_out="func_tests/data/pos_${num_file}_out.txt"
    pos_args="func_tests/data/pos_${num_file}_args.txt"
done


if [[ $1 != "-s" ]]; then
    echo -e "\n${H}Negative test:${FORM}"
fi
iter=1 
num_file="0${iter}"

neg_in="func_tests/data/neg_${num_file}_in.txt"
neg_args="func_tests/data/neg_${num_file}_args.txt"

# Тоже самое, что и для позитивных тестов
while [[ -f $neg_in ]]
do    


    eval "cp $neg_in $infile" #необходимо копирование входного файла, так как если прогрмма будет его измять, мы потеряем данные
  	neg_in=$infile

  	./func_tests/scripts/neg_case.sh "$neg_in"
	value_return=$?
    if [[ $value_return == 0 ]]; then
        if [[ $1 != "-s" ]]; then
            echo -e "${GREEN} Test ${iter}: PASS ${FORM}"
        fi
    fi
    if [[ $value_return == 1 ]]; then
        if [[ $1 != "-s" ]]; then
            echo -e "${RED} Test ${iter}: FAIL MEMORY ${FORM}" 
            err_neg=$((err_neg + 1))      
        else
            err_neg=$((err_neg + 1))
        fi
    fi
    if [[ $value_return == 2 ]]; then
        if [[ $1 != "-s" ]]; then
            echo -e "${RED} Test ${iter}: FAIL TEST (MEM ok) ${FORM}" 
            err_neg=$((err_neg + 1))      
        else
            err_neg=$((err_neg + 1))
        fi
    fi
    if [[ $value_return == 3 ]]; then
        if [[ $1 != "-s" ]]; then
            echo -e "${RED} Test ${iter}: FAIL TEST and MEM ${FORM}" 
            err_neg=$((err_neg + 1))      
        else
            err_neg=$((err_neg + 1))
        fi
    fi

    iter=$((iter + 1))
    if [[ $iter -lt 10 ]]; then
        num_file="0${iter}"
    else
        num_file="$iter"
    fi
    neg_in="func_tests/data/neg_${num_file}_in.txt"
    neg_args="func_tests/data/neg_${num_file}_args.txt"
done

if [[ $1 != "-s" ]]; then
    echo -e "\n${H}Sum failed positive test = ${err_pos} ${FORM}"
    echo -e "${H}Sum failed negative test = ${err_neg} ${FORM}"
fi

exit $((err_pos + err_neg)) 