#include "list_out.h"
#include "errors.h"

void list_printf_recurs(node_t *head)
{
    node_t *cur = head;
    if (cur == NULL)
        return;
    list_printf_recurs(head->next);
    if (cur->degree)
        printf("%d %d ", cur->smart_mult, cur->degree);
    else
        printf("%d", cur->smart_mult);
}