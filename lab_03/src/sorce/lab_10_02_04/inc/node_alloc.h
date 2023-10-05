#ifndef __NODE_CREATE_H__
#define __NODE_CREATE_H__

typedef struct node node_t;

struct node
{
    unsigned smart_mult;
    unsigned degree;

    node_t *next;
};

#include <stdlib.h>

/**
    @brief Создает новый узел связного списка.
    @param[in] smart_num Значение, которое нужно хранить в узле.
    @param[in] deg Степень, которую нужно хранить в узле.
    @return Указатель на новый узел.
    */
node_t *node_create(unsigned smart_num, unsigned deg);


#endif