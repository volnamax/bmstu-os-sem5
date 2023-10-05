#include "node_alloc.h"


node_t *node_create(unsigned smart_num, unsigned deg)
{
    node_t *node = malloc(sizeof(node_t));

    if (node)
    {
        node->smart_mult = smart_num;
        node->degree = deg;
        node->next = NULL;
    }

    return node;
}
