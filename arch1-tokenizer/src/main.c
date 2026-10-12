#include "../include/tokenizer.h"
#include "../include/history.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>

// CTRL C

int main(int argc, char const *argv[])
{

    List *history = init_history();
    char buffer[100];
    char **tokens = NULL;

    while (1)
    {

        fputs("\n> ", stdout);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        {
            continue;
        }

        remove_newln(buffer);

        tokens = tokenize(buffer);

        if (tokens == NULL || tokens[0] == NULL)
            continue;

        add_history(history, buffer);

        char *first_token = &(tokens[0][0]);

        if (is_history_cmmd(first_token))
        {
            print_history(history);
        }
        else if (is_retrieval_cmmd(first_token))
        {
            int item_id = get_numeric_val(first_token + 1);

            if (item_id == -1)
            {
                printf("\n!#: invalid option: %s", first_token);
            }
            else
            {
                char *s = get_history(history, item_id);

                if (s == NULL)
                {
                    fputs("\n!#: no such history item exists", stdout);
                }
                else
                {
                    printf("H[%d]: %s\n", item_id, s);
                    tokens = tokenize(s);
                    print_tokens(tokens);
                }
            }
        }
        else if (is_quit_cmmd(first_token))
        {
            free_tokens(tokens);
            free(tokens);
            tokens = NULL;
            break;
        }
        else if (is_clear_cmmd(first_token))
        {
            free_history(history);
            free(history);
            history = NULL;
            history = init_history();
        }
        else
        {
            print_tokens(tokens);
        }
        free_tokens(tokens);
        free(tokens);
        tokens = NULL;
    }

    free_history(history);
    free(history);
    history = NULL;

    return 0;
}