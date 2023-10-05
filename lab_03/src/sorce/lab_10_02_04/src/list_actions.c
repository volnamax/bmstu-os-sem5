#include "list_actions.h"
#include "errors.h"

// deg += deg
// Для вычияления квадрата необходимо умеличить степень в два раза
void sqr_list(node_t *head)
{
    if (!head)
        return;

    for (node_t *cur = head; cur; cur = cur->next)
        cur->degree += cur->degree;
}


node_t *del_node(node_t **head, node_t *prev_node, node_t *del_node)
{
    if (head == NULL || *head == NULL)
        return NULL;
    node_t *next_node = del_node->next;
    // del head
    if (prev_node == del_node)
    {
        if (next_node)
        {
            *head = next_node;
            free(del_node);
            return *head;
        }
        else
            free(del_node);
        return NULL;
    }
    // del next null
    else if (!next_node)
    {
        prev_node->next = NULL;
        free(del_node);
        return prev_node;
    }
    // del else node
    else
    {
        prev_node->next = next_node;
        free(del_node);
        return next_node;
    }
    return NULL;
}

void list_add_one_node_with_next(node_t **head, node_t **node)
{
    node_t *temp_next = (*node)->next;
    (*node)->next = NULL;
    *head = list_add_end(*head, *node);
    *node = temp_next;
}

// умножение чисел можно разделить на несколько этапов
// если простые числа совпадают, то просто складываем степени этих простых чисел и перенастраиваем ссылки в результирующий список 
// если не совпали то нужно выбрать наименьшее простое число и перенастраиваем ссылки в результирующий список
// если один из списков закончился ну нужно добавить все остальные узлы от другого списка в результирующий список
void mul_list(node_t *head_a, node_t *head_b, node_t **head_c)
{
    node_t *cur_b = head_b;
    node_t *next_temp = NULL;

    for (node_t *cur_a = head_a; cur_a;)
    {
        for (; cur_b;)
        {
            // if list a end
            if (!cur_a)
                list_add_one_node_with_next(head_c, &cur_b);
            // sum deg and push one node and free another node
            else if (cur_a->smart_mult == cur_b->smart_mult)
            {
                cur_b->degree += cur_a->degree;
                list_add_one_node_with_next(head_c, &cur_b);

                next_temp = cur_a->next;
                free(cur_a);
                cur_a = next_temp;
                continue;
            }
            // push list c less
            else if (cur_a->smart_mult > cur_b->smart_mult)
                list_add_one_node_with_next(head_c, &cur_b);
            else if (cur_a->smart_mult < cur_b->smart_mult)
                list_add_one_node_with_next(head_c, &cur_a);
        }
        // if list b end
        if (!cur_b && cur_a)
            list_add_one_node_with_next(head_c, &cur_a);
    }
}

bool is_lista_big_eq_listb(node_t *a, node_t *b)
{
    int count_a = 0, count_b = 0;
    bool is_eq_smart_num = false;

    for (node_t *cur_b = b; cur_b; count_b++, cur_b = cur_b->next)
    {
        for (node_t *cur_a = a; cur_a; count_a++, cur_a = cur_a->next)
        {
            if (cur_a->smart_mult == cur_b->smart_mult)
                is_eq_smart_num = true;
        }
        if (!is_eq_smart_num)
            return false;
        else
            is_eq_smart_num = false;
    }

    return (count_a >= count_b ? true : false);
}

// для начала нужно проверить что число б является подмножеством множества это делается при помощи функции is_lista_big_eq_listb
// по сути просто все простые числа числа б должны совпадать с простыми числами числа а 
// сам флгоритм деление просто это вычитание степени б из степени а, если эта степень получилась отрицательной то деление будет с остатком 
// Если получился ноль то необходимо удалить этот узел и перейти к следующему
int div_lists(node_t **a, node_t **b)
{
    if (a == NULL || *a == NULL || b == NULL || *b == NULL)
        return ERR_POINT;

    if (!is_lista_big_eq_listb(*a, *b))
        return ERR_DIV;

    int flag_div = 0;
    int tem_div = 0;
    node_t *cur_a = *a;
    node_t *cur_a_prev = *a;
    bool flag_del = false;
    int last_node = 0;

    while (cur_a)
    {
        for (node_t *cur_b = *b; cur_b; cur_b = cur_b->next)
        {
            if (cur_a->smart_mult == cur_b->smart_mult)
            {
                tem_div = cur_a->degree - cur_b->degree;
                if (tem_div < 0)
                    return ERR_DIV; // integer division is not possible
                if (tem_div == 0 && flag_div)
                {
                    cur_a = del_node(a, cur_a_prev, cur_a);
                    flag_del = true; // check node if del cur but after del cur = cur->next
                                     // if del last node this node need check
                    if (cur_a->next == NULL)
                        last_node++;
                }
                else
                    cur_a->degree = tem_div;
                flag_div++;
                break;
            }
        }
        // transition next
        if (!flag_del)
        {
            cur_a_prev = cur_a;
            cur_a = cur_a->next;
        }
        else
        {
            flag_del = false; // after deletion already moved on to the next one
            // need if last node
            if (last_node == 2)
                break;
        }
    }
    // if not one prime number did not match in the lists, it is not possible to divide completely
    if (flag_div > 0)
        return PASS;
    return ERR_DIV;
}

unsigned int get_deg_smart_num(unsigned *num, unsigned *div)
{
    unsigned deg = 0;
    while (*num % *div == 0)
    {
        *num = *num / *div;
        deg++;
    }
    return deg;
}


node_t *list_add_end(node_t *head, node_t *new_node)
{
    node_t *cur = head;
    if (!head)
        return new_node;

    for (; cur->next; cur = cur->next);

    cur->next = new_node;

    return head;
}
