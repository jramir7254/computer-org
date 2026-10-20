#include "utils.h"
#include "tokenizer.h"
#include <stdio.h>

#define LIMIT 5

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
