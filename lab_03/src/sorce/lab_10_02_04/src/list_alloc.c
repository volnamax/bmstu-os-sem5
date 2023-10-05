#include "list_alloc.h"
#include "errors.h"
#include "list_actions.h"
#include "io.h"

int list_create_ex(node_t **head, unsigned num)
{
    if (!head)
        return ERR_POINT;
    int rc = PASS;

    node_t *cur = NULL;
    unsigned deg = 0, div = 2;

    cur = node_create(1, 0);
    if (cur)
        *head = list_add_end(*head, cur);
    else
        rc = ERR_ALLOC;

    while (num > 1 && !rc)
    {
        deg = get_deg_smart_num(&num, &div);
        if (deg)
        {
            cur = node_create(div, deg);
            if (cur)
                *head = list_add_end(*head, cur);
            else
                rc = ERR_ALLOC;
        }
        if (div == 2)
            div++;
        else
            div += 2;
    }
    if (rc)
        list_free(*head);
    return rc;
}


void list_free(node_t *head)
{
    node_t *next = NULL;
    while (head != NULL)
    {
        next = head->next;
        free(head);
        head = next;
    }
    head = NULL;
}
