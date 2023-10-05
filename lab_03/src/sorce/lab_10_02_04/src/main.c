

#include "errors.h"
#include "list_alloc.h"
#include "io.h"
#include "list_actions.h"
#include "list_out.h"


int main(void)
{
    int rc = PASS;
    int action = PASS;

    action = read_action(&rc);
    if (rc)
        return rc;

    unsigned num = read_num(&rc);
    if (rc)
        return rc;

    node_t *head = NULL;
    node_t *head1 = NULL;
    unsigned num1;
    if (action == ACTION_DIV || action == ACTION_MUL)
    {
        num1 = read_num(&rc);
        if (rc)
            return rc;
        rc = list_create_ex(&head1, num1);
        if (rc)
            return rc; // last return
    }

    rc = list_create_ex(&head, num);
    if (!rc)
    {
        if (action == ACTION_OUT)
        {
            list_printf_recurs(head);
            list_free(head);
        }
        else if (action == ACTION_SQR)
        {
            sqr_list(head);
            list_printf_recurs(head);
            list_free(head);
        }
        else if (action == ACTION_MUL)
        {
            node_t *res_h = NULL;
            mul_list(head, head1, &res_h);

            list_printf_recurs(res_h);
            list_free(res_h);
        }
        else if (action == ACTION_DIV)
        {
            rc = div_lists(&head, &head1);
            if (!rc)
                list_printf_recurs(head);
            
            list_free(head);
            list_free(head1);
        }
        else
            rc = ERR_ACTION;
    }
    else
        list_free(head1);

    return rc;
}