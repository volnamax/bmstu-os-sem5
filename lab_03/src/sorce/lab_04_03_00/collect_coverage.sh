#!/bin/bash

cd func_tests/scripts/ || exit
./func_tests.sh
cd ../../
gcov main.c