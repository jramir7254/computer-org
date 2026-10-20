#include "../include/history.h"
#include "../include/tokenizer.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>

static int g_id = 0;

List *init_history()
{
    List *history = malloc(sizeof(List));
    history->root = NULL;
    return history;
}

void add_history(List *list, char *str)
{

    if (list == NULL)
    {
        return;
    }

    Item *new_item = malloc(sizeof(Item));
    new_item->id = g_id++;
    new_item->str = copy_str(str, str_len(str));
    new_item->next = NULL;

    if (list->root == NULL)
    {
        list->root = new_item;
        return;
    }

    Item *temp = list->root;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }

    temp->next = new_item;
}

char *get_history(List *list, int id)
{
    if (list == NULL)
    {
        return NULL;
    }

    Item *temp = list->root;
    while (temp != NULL)
    {
        if (temp->id == id)
        {
            return temp->str;
        }
        temp = temp->next;
    }

    return NULL;
}

void print_history(List *list)
{

    if (list == NULL)
    {
        return;
    }

    Item *temp = list->root;
    while (temp != NULL)
    {
        printf("%cH[%d]: %s", temp->id ? '\n' : '\0', temp->id, temp->str);
        temp = temp->next;
    }
}

void free_history(List *list)
{
    g_id = 0;

    if (list == NULL || list->root == NULL)
        return;

    Item *current = list->root;
    Item *next_item;

    while (current != NULL)
    {
        next_item = current->next;
        free(current->str);
        free(current);
        current = next_item;
    }

    list->root = NULL;
}