#include "tokenizer.h"
#include "history.h"
#include "utils.h"
#include "strings.h"
#include "commands.h"

#include <stdio.h>
#include <stdlib.h>

// CTRL C

int main()
{

    List *history = init_history();
    char buffer[100];
    char **tokens = NULL;

    Input_Type type = TEXT;

    print_help_cmd();

    while (type != EXIT)
    {

        fputs("\n> ", stdout);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        {
            continue;
        }

        str_remove_newln(buffer);

        tokens = tokenize(buffer);

        if (tokens == NULL || tokens[0] == NULL)
            continue;

        add_history(history, buffer);

        type = get_input_type(tokens);

        switch (type)
        {
        case EXIT:
            fputs("\nfreeing tokens...", stdout);
            break;
        case HELP:
            print_help_cmd();
            break;
        case HISTORY:
            print_history(history);
            break;
        case HISTORY_CLEAR:
            free_history(history);
            free(history);
            history = NULL;
            history = init_history();
            break;
        case HISTORY_GET:;
            int item_id = get_numeric_val(&(tokens[1][0]));

            if (item_id == -1)
            {
                printf("\nhistory: invalid option: %s", &(tokens[1][0]));
                break;
            }

            char *req_item = get_history(history, item_id);

            if (req_item == NULL)
            {
                fputs("\nhistory: no such history item exists", stdout);
                break;
            }
            else
            {
                printf("\nH[%d]: %s\n", item_id, req_item);
                tokens = tokenize(req_item);
                print_tokens(tokens);
            }
            break;
        case DEFAULT_GET:;
            int item_id_2 = get_numeric_val(&(tokens[0][1]));

            if (item_id_2 == -1)
            {
                printf("\n!#: invalid option: %s", &(tokens[0][1]));
                break;
            }

            char *req_item_2 = get_history(history, item_id_2);

            if (req_item_2 == NULL)
            {
                fputs("\n!#: no such history item exists", stdout);
                break;
            }
            else
            {
                printf("\nH[%d]: %s\n", item_id_2, req_item_2);
                tokens = tokenize(req_item_2);
                print_tokens(tokens);
            }
            break;
        default:
            print_tokens(tokens);
        }

        free_tokens(tokens);
        free(tokens);
        tokens = NULL;
    }

    fputs("\nfreeing history...", stdout);
    free_history(history);
    free(history);
    history = NULL;
    fputs("\nBye!", stdout);

    return 0;
}