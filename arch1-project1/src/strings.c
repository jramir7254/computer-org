#include "strings.h"
#include "tokenizer.h"

int str_equals(char *str, const char *comp)
{
    while (*str != '\0')
    {
        if (*str++ != *comp++)
        {
            return 0;
        }
    }
    return 1;
}

int str_starts_with(char *str, char comp)
{
    return *str == comp;
}

void str_remove_newln(char *str)
{
    while (*str != '\n')
    {
        str++;
    }

    *str = '\0';
}

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