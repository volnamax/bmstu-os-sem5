#include "check_main.h"
#include "list_actions.h"
#include <stdbool.h>
#include <string.h>
#include "errors.h"

#define ERR 1

node_t *create_list(int const *arr_smart, int const *arr_deg, size_t count_el)
{
    node_t *head = NULL;
    node_t *temp_node = NULL;

    size_t i = 0;
    while (i < count_el)
    {

        temp_node = node_create(arr_smart[i], arr_deg[i]);
        if (temp_node == NULL)
        {
            list_free(head);
            return NULL;
        }
        head = list_add_end(head, temp_node);
        i++;
    }
    return head;
}

bool is_eq_list(node_t *head_a, node_t *head_b)
{
    if (head_a == NULL || head_b == NULL)
        return NULL;

    node_t *cur_a = head_a, *cur_b = head_b;
    for (; cur_a && cur_b; cur_a = cur_a->next, cur_b = cur_b->next)
    {
        if (cur_a->smart_mult != cur_b->smart_mult || cur_a->degree != cur_b->degree)
            return false;
    }
    if (cur_a != NULL || cur_b != NULL)
        return false;

    return true;
}

// Негативный тест на нулевой указатель

START_TEST(test_neg_div_two_null_point_list)
{
    int rc = PASS;

    rc = div_lists(NULL, NULL);
    ck_assert_int_eq(rc, ERR_POINT);

    END_TEST
}

// Негативный тест на нулевой указатель
START_TEST(test_neg_div_one_null_point_list)
{
    int rc = PASS;
    node_t *head = node_create(1, 1);

    if (head == NULL)
        ck_assert_int_eq(rc, ERR);

    rc = div_lists(&head, NULL);
    free(head);
    ck_assert_int_eq(rc, ERR_POINT);

    END_TEST
}

// Негативный тест на нулевой указатель
START_TEST(test_neg_div_null_point_list)
{
    int rc = PASS;
    node_t *head = node_create(1, 1);

    if (head == NULL)
        ck_assert_int_eq(rc, ERR);

    rc = div_lists(NULL, &head);
    free(head);
    ck_assert_int_eq(rc, ERR_POINT);

    END_TEST
}

// Негативный тест на проверку делимости
START_TEST(test_neg_div_is)
{
    int rc = PASS;
    int count_a = 5;
    int count_b = 6;

    int arr_a[] = {1, 2, 3, 4, 5};
    int arr_b[] = {1, 2, 3, 4, 5, 6};

    node_t *head_a = create_list(arr_a, arr_a, count_a);
    if (head_a == NULL)
        ck_assert_int_eq(rc, ERR);

    node_t *head_b = create_list(arr_b, arr_b, count_b);
    if (head_b == NULL)
        ck_assert_int_eq(rc, ERR);

    rc = div_lists(&head_a, &head_b);
    list_free(head_a);
    list_free(head_b);

    ck_assert_int_eq(rc, ERR_DIV);

    END_TEST
}

// Позитивный тест на деление
START_TEST(test_pos_div)
{
    int rc = PASS;
    int count_a = 5;
    int count_b = 6;

    int arr_a[] = {1, 2, 3, 4, 5};
    int arr_b[] = {1, 2, 3, 4, 5, 6};

    node_t *head_a = create_list(arr_a, arr_a, count_a);
    if (head_a == NULL)
        ck_assert_int_eq(rc, ERR);

    node_t *head_b = create_list(arr_b, arr_b, count_b);
    if (head_b == NULL)
        ck_assert_int_eq(rc, ERR);

    rc = div_lists(&head_b, &head_a);

    list_free(head_a);
    list_free(head_b);
    ck_assert_int_eq(rc, PASS);

    END_TEST
}

// Позитивный тест на деление c 1 эл
START_TEST(test_pos_div_one_el)
{
    int rc = PASS;
    int count_a = 1;
    int count_b = 1;
    int count_res = 1;

    int arr_a[] = {1};
    int arr_deg[] = {1};
    int arr_deg_res[] = {0};

    int arr_b[] = {1};
    int arr_res[] = {1};

    node_t *head_res = create_list(arr_res, arr_deg_res, count_res);
    if (head_res == NULL)
        ck_assert_int_eq(rc, ERR);

    node_t *head_a = create_list(arr_a, arr_deg, count_a);
    if (head_a == NULL)
        ck_assert_int_eq(rc, ERR);

    node_t *head_b = create_list(arr_b, arr_b, count_b);
    if (head_b == NULL)
        ck_assert_int_eq(rc, ERR);

    rc = div_lists(&head_a, &head_b);
    // ck_abort_msg("__%d___%d %d", head_a->degree, head_res->degree, rc);
    if (!is_eq_list(head_a, head_res))
        ck_assert_int_eq(0, ERR);

    list_free(head_a);
    list_free(head_b);
    list_free(head_res);

    ck_assert_int_eq(rc, PASS);

    END_TEST
}

// Позитивный тест на деление c 2 эл
START_TEST(test_pos_div_two_el)
{
    int rc = PASS;
    int count = 2;

    int arr_a[] = {1, 2};
    int arr_deg_a[] = {1, 2};

    int arr_b[] = {1, 2};
    int arr_b_deg[] = {1, 1};

    int arr_res[] = {1, 2};
    int arr_res_deg[] = {0, 1};

    node_t *head_res = create_list(arr_res, arr_res_deg, count);
    if (head_res == NULL)
        ck_assert_int_eq(rc, ERR);

    node_t *head_a = create_list(arr_a, arr_deg_a, count);
    if (head_a == NULL)
        ck_assert_int_eq(rc, ERR);

    node_t *head_b = create_list(arr_b, arr_b_deg, count);
    if (head_b == NULL)
        ck_assert_int_eq(rc, ERR);

    rc = div_lists(&head_a, &head_b);

    if (!is_eq_list(head_a, head_res))
        ck_assert_int_eq(0, ERR);

    list_free(head_a);
    list_free(head_b);
    list_free(head_res);

    ck_assert_int_eq(rc, PASS);

    END_TEST
}

// Позитивный тест на деление c 5 эл
START_TEST(test_pos_div_five_el)
{
    int rc = PASS;
    int count = 5;

    int arr_a[] = {1, 2, 3, 5, 17};
    int arr_deg_a[] = {1, 2, 2, 2, 2};

    int arr_b[] = {1, 2, 3, 5, 17};
    int arr_b_deg[] = {1, 1, 1, 1, 1};

    int arr_res[] = {1, 2, 3, 5, 17};
    int arr_res_deg[] = {0, 1, 1, 1, 1};

    node_t *head_res = create_list(arr_res, arr_res_deg, count);
    if (head_res == NULL)
        ck_assert_int_eq(rc, ERR);

    node_t *head_a = create_list(arr_a, arr_deg_a, count);
    if (head_a == NULL)
        ck_assert_int_eq(rc, ERR);

    node_t *head_b = create_list(arr_b, arr_b_deg, count);
    if (head_b == NULL)
        ck_assert_int_eq(rc, ERR);

    rc = div_lists(&head_a, &head_b);

    if (!is_eq_list(head_a, head_res))
        ck_assert_int_eq(0, ERR);

    list_free(head_a);
    list_free(head_b);
    list_free(head_res);

    ck_assert_int_eq(rc, PASS);

    END_TEST
}

/////////////////////////////////////////////////////////////////////////////

// Негативный тест на нулевой указатель
START_TEST(test_neg_sqr_list_null_point_list)
{
    sqr_list(NULL);
    ck_assert_int_eq(0, PASS);

    END_TEST
}

// позитивный  тест на возведение в кв 5 элементов
START_TEST(test_pos_sqr_list_five_el)
{
    int rc = PASS;
    int count = 5;

    int arr_a[] = {1, 2, 3, 5, 17};
    int arr_deg_a[] = {1, 2, 2, 2, 2};

    int arr_res[] = {1, 2, 3, 5, 17};
    int arr_res_deg[] = {2, 4, 4, 4, 4};

    node_t *head_res = create_list(arr_res, arr_res_deg, count);
    if (head_res == NULL)
        ck_assert_int_eq(rc, ERR);

    node_t *head_a = create_list(arr_a, arr_deg_a, count);
    if (head_a == NULL)
        ck_assert_int_eq(rc, ERR);

    sqr_list(head_a);
    if (!is_eq_list(head_a, head_res))
        ck_assert_int_eq(rc, ERR);
    list_free(head_a);
    list_free(head_res);
    ck_assert_int_eq(rc, PASS);

    END_TEST
}

// позитивный  тест на возведение в кв 1 элемента
START_TEST(test_pos_sqr_list_one_el)
{
    int rc = PASS;
    int count = 1;

    int arr_a[] = {17};
    int arr_deg_a[] = {45};

    int arr_res[] = {17};
    int arr_res_deg[] = {90};

    node_t *head_res = create_list(arr_res, arr_res_deg, count);
    if (head_res == NULL)
        ck_assert_int_eq(rc, ERR);

    node_t *head_a = create_list(arr_a, arr_deg_a, count);
    if (head_a == NULL)
        ck_assert_int_eq(rc, ERR);

    sqr_list(head_a);
    if (!is_eq_list(head_a, head_res))
        ck_assert_int_eq(rc, ERR);
    list_free(head_a);
    list_free(head_res);
    ck_assert_int_eq(rc, PASS);

    END_TEST
}

/////////////////////////////////////////////////////

// Позитивный тест на умножение 5 эл
START_TEST(test_pos_mul_list_five_el)
{
    int rc = PASS;
    int count = 5;

    int arr_a[] = {1, 2, 3, 5, 17};
    int arr_deg_a[] = {1, 2, 2, 2, 2};

    int arr_b[] = {1, 2, 3, 5, 17};
    int arr_b_deg[] = {1, 1, 1, 1, 1};

    int arr_res[] = {1, 2, 3, 5, 17};
    int arr_res_deg[] = {2, 3, 3, 3, 3};

    node_t *head_res = create_list(arr_res, arr_res_deg, count);
    if (head_res == NULL)
        ck_assert_int_eq(rc, ERR);

    node_t *head_a = create_list(arr_a, arr_deg_a, count);
    if (head_a == NULL)
        ck_assert_int_eq(rc, ERR);

    node_t *head_b = create_list(arr_b, arr_b_deg, count);
    if (head_b == NULL)
        ck_assert_int_eq(rc, ERR);

    node_t *head_c = NULL;
    mul_list(head_a, head_b, &head_c);

    if (!is_eq_list(head_c, head_res))
        ck_assert_int_eq(rc, ERR);

    list_free(head_c);
    list_free(head_res);

    ck_assert_int_eq(rc, PASS);

    END_TEST
}

// Позитивный тест на умножение 1 эл
START_TEST(test_pos_mul_list_one_el)
{
    int rc = PASS;
    int count = 1;

    int arr_a[] = {3};
    int arr_deg_a[] = {12};

    int arr_b[] = {5};
    int arr_b_deg[] = {1};

    int arr_res[] = {3, 5};
    int arr_res_deg[] = {12, 1};

    node_t *head_res = create_list(arr_res, arr_res_deg, count + 1);
    if (head_res == NULL)
        ck_assert_int_eq(rc, ERR);

    node_t *head_a = create_list(arr_a, arr_deg_a, count);
    if (head_a == NULL)
        ck_assert_int_eq(rc, ERR);

    node_t *head_b = create_list(arr_b, arr_b_deg, count);
    if (head_b == NULL)
        ck_assert_int_eq(rc, ERR);

    node_t *head_c = NULL;
    mul_list(head_a, head_b, &head_c);
    // ck_abort_msg("%d__%d", head_res->next->smart_mult, head_c->next->degree);
    if (!is_eq_list(head_c, head_res))
        ck_assert_int_eq(rc, ERR);

    list_free(head_c);
    list_free(head_res);

    ck_assert_int_eq(rc, PASS);

    END_TEST
}

Suite *list_actions(void)
{
    Suite *s;
    TCase *tc_neg, *tc_pos;

    s = suite_create("list_actions");
    tc_pos = tcase_create("posituves");
    tcase_add_test(tc_pos, test_pos_div);
    tcase_add_test(tc_pos, test_pos_div_one_el);
    tcase_add_test(tc_pos, test_pos_div_two_el);
    tcase_add_test(tc_pos, test_pos_div_five_el);

    tcase_add_test(tc_pos, test_pos_sqr_list_five_el);
    tcase_add_test(tc_pos, test_pos_sqr_list_one_el);

    tcase_add_test(tc_pos, test_pos_mul_list_five_el);
    tcase_add_test(tc_pos, test_pos_mul_list_one_el);

    suite_add_tcase(s, tc_pos);

    tc_neg = tcase_create("negatives");

    tcase_add_test(tc_neg, test_neg_div_null_point_list);
    tcase_add_test(tc_neg, test_neg_div_one_null_point_list);
    tcase_add_test(tc_neg, test_neg_div_two_null_point_list);
    tcase_add_test(tc_neg, test_neg_div_is);

    tcase_add_test(tc_neg, test_neg_sqr_list_null_point_list);

    suite_add_tcase(s, tc_neg);

    return s;
}