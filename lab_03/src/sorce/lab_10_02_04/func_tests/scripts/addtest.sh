#!/bin/bash
count_add_pos=$1
count_add_neg=$2

cd ../../ || exit 255

iter=1 
num_file="0${iter}"
pos_in="func_tests/data/pos_${num_file}_in.txt"
pos_out="func_tests/data/pos_${num_file}_out.txt"

while [[ -f $pos_in && -f $pos_out ]]
do 

    pos_in="func_tests/data/pos_${num_file}_in.txt"
    pos_out="func_tests/data/pos_${num_file}_out.txt"
    iter=$((iter + 1))
    if [[ $iter -lt 10 ]]; then
        num_file="0${iter}"
    else
        num_file="$iter"
    fi
done 

iter=$((iter - 1))
num_file="0${iter}"
while [[ $count_add_pos != 0 ]]
do
    pos_in="func_tests/data/pos_${num_file}_in.txt"
    pos_out="func_tests/data/pos_${num_file}_out.txt"
    pos_arg="func_tests/data/pos_${num_file}_args.txt"
    pos_tmp="func_tests/data/temp.temp"
    eval "touch $pos_out"
    eval "touch $pos_in"
    eval "echo f 1 $pos_in $pos_tmp  > $pos_arg"

    count_add_pos=$((count_add_pos - 1))

    # Это условие нужно для  номера в имени если он меньше 10
    iter=$((iter + 1))
    if [[ $iter -lt 10 ]]; then
        num_file="0${iter}"
    else
        num_file="$iter"
    fi
done

if [[ "$#" == 2 ]]; then
iter=1 
num_file="0${iter}"
neg_in="func_tests/data/neg_${num_file}_in.txt"

while [[ -f $neg_in  ]]
do 

    neg_in="func_tests/data/neg_${num_file}_in.txt"
    iter=$((iter + 1))
    if [[ $iter -lt 10 ]]; then
        num_file="0${iter}"
    else
        num_file="$iter"
    fi
done 

iter=$((iter - 1))
num_file="0${iter}"

while [[ $count_add_neg != 0 ]]
do
    neg_in="func_tests/data/neg_${num_file}_in.txt"
    neg_arg="func_tests/data/neg_${num_file}_args.txt"
    neg_tmp="func_tests/data/temp.temp"

    eval "touch $neg_in"
    eval "touch $neg_arg"
    eval "echo f 1 $neg_in $neg_tmp > $neg_arg"

    count_add_neg=$((count_add_neg - 1))

    # Это условие нужно для  номера в имени если он меньше 10
    iter=$((iter + 1))
    if [[ $iter -lt 10 ]]; then
        num_file="0${iter}"
    else
        num_file="$iter"
    fi
done
fi