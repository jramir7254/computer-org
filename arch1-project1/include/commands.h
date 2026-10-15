#ifndef COMMANDS_H
#define COMMANDS_H

typedef enum Input_Type
{
    HISTORY,
    HISTORY_CLEAR,
    HISTORY_GET,
    DEFAULT_GET,
    GET,
    EXIT,
    HELP,
    TEXT
} Input_Type;

Input_Type get_input_type(char **tokens);

void print_help_cmd();

#endif