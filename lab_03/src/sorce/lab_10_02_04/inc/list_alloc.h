#ifndef __LIST_CREATE_H__
#define __LIST_CREATE_H__

#include "node_alloc.h"
#include <stdio.h>
#include <stdlib.h>

/**

    @brief Создает связный список 
    @param[out] head Указатель на начало связного списка.
    @param[in] num число.
    @return 0 в случае успеха, ненулевое значение в случае ошибки.
    */
int list_create_ex(node_t **head, unsigned num);


/**

    @brief Освобождает память, занимаемую связным списком.
    @param[in] head Начало связного списка.
    */
void list_free(node_t *head);



#endif