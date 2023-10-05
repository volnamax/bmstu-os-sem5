/*Для решения этой задачи нужно использовать функции стандартной библиотеки.
Требуется написать программу, которая запрашивает у пользователя строку и разби-
вает оную на слова. В результате разбора строки должен быть сформирован массив слов.
Из слов, отличных от последнего, составляется новая строка. Слова в результирующую
строку помещаются в обратном порядке с одним пробелом в качестве разделителя. После
последнего слова пробел не добавляется.
Прежде, чем очередное слово помещается в результирующую строку, оно подвергается
преобразованию в соответствии с вариантом.
Если результирующая строка не пустая, она выводится на экран следующим образом
printf("Result:␣%s\n", str)
Удалить из слова все последующие вхождения первой буквы.*/
#include <stdio.h>
#include <string.h>

#define PASS 0
#define ERROR_LEN 1
#define ERROR_TASK 2
#define ERROR_INPUT 3
#define ERROR_EMPTY_STR 5
#define ERROR_COUNT_WORD 6

#define START_DEL 1
#define MAX_LEN_WORD 17
#define MAX_WORD 16
#define N 257
#define SEPARATORS " ,;:-.!?"

int stread(char *str, int *str_len);
int strsplit_arr(char *str, char (*word)[MAX_LEN_WORD], int *count_w);
void delchar_instr(char str[MAX_LEN_WORD], char ch, int pos_start);
int revstr_withut_lstwrd_frstleter(char strmatr[][MAX_LEN_WORD], int count_word, char *str);

int main()
{
    int count_word = 0, lenstr = 0;
    int rc = PASS;
    char string[N];
    char lexem_matrix[N / 2][MAX_LEN_WORD];

    rc = stread(string, &lenstr);
    if (rc == PASS)
    {
        strsplit_arr(string, lexem_matrix, &count_word);
        if (rc == PASS && count_word > 1)
        {
            char newstr[N] = { 0 };
            rc = revstr_withut_lstwrd_frstleter(lexem_matrix, count_word, newstr);
            if (rc == PASS)
                printf("Result: %s", newstr);
            else
                return rc;
        }
        else
            rc = ERROR_TASK;
    }
    return rc;
}

int stread(char *str, int *str_len)
{
    char temp_str[N + 1];
    char *ch = fgets(temp_str, N + 1, stdin);

    if (ch == NULL)
        return ERROR_INPUT;
    if (temp_str[0] == '\n')
        return ERROR_EMPTY_STR;
    int len_temp_str = strlen(temp_str);
    if ((len_temp_str == N) && (temp_str[len_temp_str - 1] != '\n'))
        return ERROR_LEN;

    if (temp_str[len_temp_str - 1] == '\n')
        temp_str[len_temp_str - 1] = '\0';

    strcpy(str, temp_str);
    *str_len = len_temp_str - 1;

    return PASS;
}

int strsplit_arr(char *str, char (*word)[MAX_LEN_WORD], int *count_w)
{
    char *token = strtok(str, SEPARATORS);
    int i = 0;

    while (token != NULL)
    {
        if (i > MAX_WORD)
            return ERROR_COUNT_WORD;
            
        if (strlen(token) + 1 <= MAX_LEN_WORD)
        {
            strcpy(word[i++], token);
            token = strtok(NULL, SEPARATORS);
        }
        else 
            return ERROR_LEN;
    }
    if (i < 1)
        return ERROR_TASK;

    *count_w = i;
    return PASS;
}

int revstr_withut_lstwrd_frstleter(char strmatr[][MAX_LEN_WORD], int count_word, char *str)
{
    int len = 0;
    for (int i = count_word - 1; i > -1; i--)
        if (strncmp(strmatr[i], strmatr[count_word - 1], MAX_LEN_WORD) != 0)
        {
            delchar_instr(strmatr[i], strmatr[i][0], START_DEL);
            len = strlen(strmatr[i]);
            strncat(str, strmatr[i], len);
            strcat(str, " ");
        }
    if (len == 0)
        return ERROR_COUNT_WORD;

    return PASS;
}

// функция для удаления определенного символа в слове начиная с определенной позиции в слове
void delchar_instr(char str[MAX_LEN_WORD], char ch, int pos_start)
{
    int k = 0;
    for (int j = pos_start; str[j] != '\0'; j++)
        //  если первый символ равен последующим
        if (str[j] == ch)
        {
            k = j;
            while (str[k])
            {
                str[k] = str[k + 1];
                k++;
            }
            j--;
        }
}