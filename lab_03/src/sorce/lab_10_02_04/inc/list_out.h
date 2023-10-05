#ifndef __LIST_OUT_H__
#define __LIST_OUT_H__
#include <stdio.h>
#include "node_alloc.h"

/**

    @brief Рекурсивно выводит содержимое связного списка.
    @param[in] head Начало связного списка.
    */
void list_printf_recurs(node_t *head);

#endif