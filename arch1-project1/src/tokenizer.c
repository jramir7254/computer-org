#include "../include/tokenizer.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>

#define DEBUG 0
#define TERMINATOR '\0'

int is_space_char(char c)
{
    return c == '\t' || c == ' ' || c == '\n';
}

int is_non_space_char(char c)
{
    return c != '\t' && c != ' ' && c != TERMINATOR;
}

char *token_start(char *str)
{
    while (is_space_char(*str))
    {
        str++;
    }
    return str;
}

char *token_terminator(char *token)
{
    DEBUG && (printf("\ntoken addr: %p char val: %c", token, *token));

    while (is_non_space_char(*token))
    {
        token++;
        DEBUG && (printf("\ntoken addr: %p char val: %c", token, *token));
    }

    return token;
}

int count_tokens(char *str)
{
    int count = 0;

    while (*str != TERMINATOR)
    {
        str = token_start(str);
        if (*str == TERMINATOR)
            return count;
        count++;
        str = token_terminator(str);
    }

    return count;
}

char *copy_str(char *inStr, short len)
{
    char *str = malloc(len + 1);

    if (str == NULL)
    {
        fprintf(stderr, "\nMemory allocation failed in *copy_str(char *inStr, short len)");
        return NULL;
    }

    for (int i = 0; i < len; i++)
    {
        str[i] = inStr[i];
    }

    return str;
}

char **tokenize(char *str)
{

    int num_tokens = count_tokens(str);

    if (num_tokens <= 0)
    {
        return NULL;
    }

    char **tokens = malloc((num_tokens + 1) * sizeof(char *));

    if (tokens == NULL)
    {
        fprintf(stderr, "\nMemory allocation failed in **tokenize()");
        return NULL;
    }

    short token_length = 0;
    int tokens_index = 0;

    while (tokens_index < num_tokens)
    {
        str = token_start(str);

        if (*str == TERMINATOR)
            return tokens;

        token_length = token_len(str);

        tokens[tokens_index++] = copy_str(str, token_length);

        str = token_terminator(str);
    }

    tokens[num_tokens] = NULL;

    return tokens;
}

void print_tokens(char **tokens)
{
    for (int i = 0; tokens[i] != NULL; i++)
    {
        printf("%cT[%d]: %s", i ? '\n' : '\0', i, tokens[i]);
    }
}

void free_tokens(char **tokens)
{
    for (int i = 0; tokens[i] != NULL; i++)
    {
        free(tokens[i]);
    }
}