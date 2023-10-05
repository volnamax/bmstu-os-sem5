#include "io.h"
#include "errors.h"

int read_action(int *rc)
{
    char str[MAX_STR];
    if (scanf("%s", str) != EOF)
    {
        if (strlen(str) == 3)
        {
            if (strcmp(str, "out") == 0)
                return ACTION_OUT;
            else if (strcmp(str, "mul") == 0)
                return ACTION_MUL;
            else if (strcmp(str, "div") == 0)
                return ACTION_DIV;
            else if (strcmp(str, "sqr") == 0)
                return ACTION_SQR;
            else
                *rc = ERR_ACTION;
        }
        else
            *rc = ERR_ACTION;
    }
    else
        *rc = ERR_ACTION;
    return ERR_ACTION;
}

int read_num(int *rc)
{
    int temp_num;

    if (scanf("%d", &temp_num) == 1 && temp_num > 0)
        return temp_num;
    else
        *rc = ERR_NUM;
    return ERR_NUM;
}
