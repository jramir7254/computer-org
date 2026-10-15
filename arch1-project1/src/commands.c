#include <stdio.h>
#include "commands.h"
#include "strings.h"

Input_Type get_input_type(char **tokens)
{
    char *first_token = &(tokens[0][0]);

    if (str_equals(first_token, "exit"))
        return EXIT;
    if (str_equals(first_token, "help"))
        return HELP;
    if (str_starts_with(first_token, '!'))
        return DEFAULT_GET;

    char *second_token = &(tokens[1][0]);

    if (str_equals(first_token, "history"))
    {
        if (second_token == NULL)
            return HISTORY;

        if (str_equals(second_token, "clear"))
            return HISTORY_CLEAR;

        return HISTORY_GET;
    }

    return TEXT;
}

void print_help_cmd()
{
    printf("Usage: <command> [args]\n\n");
    printf("Commands:\n");
    printf("  %-12s %s\n", "help", "prints this page");
    printf("  %-12s %s\n", "exit", "exits the program");
    printf("  %-12s %s\n", "!<id>", "get history item by id");
    printf("  %-12s %s\n", "history", "view history");
    printf("    %-10s %s\n", "clear", "clear history");
    printf("    %-10s %s\n", "<id>", "get history item by id");
}
