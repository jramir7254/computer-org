#include "../include/utils.h"
#include "../include/tokenizer.h"
#include <stdio.h>

#define ZERO '0' // 48
#define NINE '9' // 57
#define LIMIT 5

short str_len(char *str)
{
    short len = 0;

    while (*(str++) != '\0')
    {
        len++;
    }

    return len;
}

short token_len(char *str)
{
    short len = 0;

    while (is_non_space_char(*(str++)))
    {
        len++;
    }

    return len;
}

int is_history_cmmd(char *str)
{
    char history[] = "history";

    for (int i = 0; i < (int)sizeof(history) - 1; i++)
    {
        if (str[i] == '\n' || str[i] == '\0' || str[i] != history[i])
        {
            return 0;
        }
    }

    return 1;
}

int is_quit_cmmd(char *str)
{
    char quit[] = "quit";

    for (int i = 0; i < (int)sizeof(quit) - 1; i++)
    {
        if (str[i] == '\n' || str[i] == '\0' || str[i] != quit[i])
        {
            return 0;
        }
    }

    return 1;
}

int is_clear_cmmd(char *str)
{
    char clear[] = "clear";

    for (int i = 0; i < (int)sizeof(clear) - 1; i++)
    {
        if (str[i] == '\n' || str[i] == '\0' || str[i] != clear[i])
        {
            return 0;
        }
    }

    return 1;
}

int is_retrieval_cmmd(char *str)
{
    return str[0] == '!';
}

int is_char_num(char c)
{
    return (c >= '0' && c <= '9');
}

int get_numeric_val(char *str)
{
    if (*str == '\n' || *str == '\0')
        return -1;

    int val = 0;
    short count = 0;

    while (*str != '\0' && *str != '\n')
    {
        if (!is_char_num(*str) || count++ > LIMIT)
        {
            return -1;
        }
        val = (val * 10) + (*str - '0');
        str++;
    }

    return val;
}

void remove_newln(char *str)
{
    while (*str != '\n')
    {
        str++;
    }

    *str = '\0';
}
