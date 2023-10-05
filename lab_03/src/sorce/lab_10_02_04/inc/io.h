#ifndef __IO_H__
#define __IO_H__

#include <string.h>
#include <stdio.h>
#include "list_alloc.h"

#define MAX_STR 128

/**

    @brief Читает целое число из входного потока.
    @param[out] rc Код ошибки (0 - успех, иначе - ошибка).
    @return Считанное число.
    */
int read_num(int *rc);

/**

    @brief Читает действие из входного потока.
    @param[out] rc Код ошибки (0 - успех, иначе - ошибка).
    @return Код считанного действия.
    */
int read_action(int *rc);



#endif