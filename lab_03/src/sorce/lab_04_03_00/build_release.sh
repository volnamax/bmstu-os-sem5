#!/bin/bash
 gcc -std=c99 -Wall -Werror -Wpedantic -Wextra -Wvla -Wfloat-equal -Wfloat-conversion -c main.c # Получаем объектный файл (компиляция)
 gcc -o app.exe main.o -lm # Получаем исполняемый файл (компановка)
