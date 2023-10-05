#!/bin/bash

file_temp_main=(
    "*.exe"
    "*.o"
    "*.gcda"
    "*.gcno"
    "*.gcov"
    "*.temp"
)

file_temp_data=(
    "*.log"
    "*.temp"
    "all_out.txt"
)

for i in ${!file_temp_main[*]}; do
    eval "find ./ -name ${file_temp_main[$i]} -delete"
done
cd func_tests/data/ || exit 1
for i in ${!file_temp_data[*]}; do
    eval "find ./ -name ${file_temp_data[$i]} -delete"
done
