#!/bin/bash
 gcc -std=c99 -Wall -Werror -Wpedantic -Wvla -Wextra -Wfloat-equal -Wfloat-conversion -c main.c # Получаем объектный файл (компиляция)
 gcc -o app.exe main.o -lm -g3 # Получаем исполняемый файл c ключем -g3 для получение доп отладочной инфы (компановка)
