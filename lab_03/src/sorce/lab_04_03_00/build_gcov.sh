#!/bin/bash
gcc -std=c99 -Wall -Werror main.c -coverage -O0 -o app.exe -lm