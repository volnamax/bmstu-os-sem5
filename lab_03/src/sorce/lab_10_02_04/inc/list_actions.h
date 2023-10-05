#ifndef __LIST_ACTIONS_H__
#define __LIST_ACTIONS_H__

#include "list_alloc.h"
#include <stdbool.h>

/**
    @brief Вычисляет  квадрат числа представленного в виде списка где узлы это разложение этого числа на простые множетели и степень.
    @param[in] head Начало связного списка.
    */
void sqr_list(node_t *head);

/**
    @brief Умножает два связных списка.
    @param[in] head_a Начало первого связного списка.
    @param[in] head_b Начало второго связного списка.
    @param[out] head_c Указатель на начало результирующего связного списка.
    */
void mul_list(node_t *head_a, node_t *head_b, node_t **head_c);

/**

    @brief производит деление чисел,числа представленного в виде списка где узлы это разложение этого числа на простые множетели и степень
    @param[in/ out] a Указатель на начало делимого связного списка.
    @param[in] b Указатель на начало делителя.
    @return код ошибки
    */
int div_lists(node_t **a, node_t **b);

/**

    @brief получает степень простого числа
    @param[in/ out] num входное число
    @param[in/ out] div простое число
    @return степень простого числа
    */
unsigned int get_deg_smart_num(unsigned *num, unsigned *div);

/**

    @brief Добавляет узел в конец связного списка.
    @param[in] head Начало связного списка.
    @param[in] new_node Узел, который нужно добавить.
    @return Начало связного списка.
    */
node_t *list_add_end(node_t *head, node_t *new_node);

#endif
