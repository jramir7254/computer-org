#include "../include/tokenizer.h"
#include "../include/history.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{

    char buffer[50];

    List *list = init_history();

    fputs("\nEnter stuff: ", stdout);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';

    add_history(list, "One");
    add_history(list, "Two");
    add_history(list, "Three");
    add_history(list, buffer);
    add_history(list, "Four");
    add_history(list, "Five");

    print_history(list);

    printf("\nget list[0]: %s", get_history(list, 0));
    printf("\nget list[1]: %s", get_history(list, 1));
    printf("\nget list[2]: %s", get_history(list, 2));
    printf("\nget list[3]: %s", get_history(list, 3));
    printf("\nget list[4]: %s", get_history(list, 4));

    free_history(list);
    free(list);
    list = NULL;

    printf("is history cmmd: %s", is_history_cmmd("history") ? "true" : "false");
    printf("\nis history cmmd: %s", is_history_cmmd("mystery") ? "true" : "false");
    printf("\nis history cmmd: %s", is_history_cmmd("") ? "true" : "false");

    printf("\nis ret cmmd: %s", is_retrieval_cmmd("!") ? "true" : "false");
    printf("\nis ret cmmd: %s", is_retrieval_cmmd("!3") ? "true" : "false");
    printf("\nis ret cmmd: %s", is_retrieval_cmmd("A") ? "true" : "false");
    printf("\nis ret cmmd: %s", is_retrieval_cmmd("") ? "true" : "false");

    printf("\nis valid id: %s", is_valid_id("23") ? "true" : "false");
    printf("\nis valid id: %s", is_valid_id("0") ? "true" : "false");
    printf("\nis valid id: %s", is_valid_id("9") ? "true" : "false");
    printf("\nis valid id: %s", is_valid_id("G") ? "true" : "false");
    printf("\nis valid id: %s", is_valid_id("G3") ? "true" : "false");
    printf("\nis valid id: %s", is_valid_id("4D") ? "true" : "false");
    printf("\nis valid id: %s", is_valid_id(" 1") ? "true" : "false");
    printf("\nis valid id: %s", is_valid_id("2 ") ? "true" : "false");
    printf("\nis valid id: %s", is_valid_id(" 3 ") ? "true" : "false");

    // printf("%d %d %d", is_space_char(' '), is_space_char('a'), is_space_char('\t'));
    // printf("\nnum tokens: %d", count_tokens("should be three"));
    // printf("\nnum tokens: %d", count_tokens("a b c "));
    // printf("\nnum tokens: %d", count_tokens("  big   spaces   "));
    // printf("\nnum tokens: %d", count_tokens("  start space"));
    // printf("\nnum tokens: %d", count_tokens("end space   "));
    // printf("\nnum tokens: %d", count_tokens("word"));
    // printf("\nnum tokens: %d", count_tokens(""));
    // printf("\nnum tokens: %d", count_tokens(" "));

    // printf("\nlen: %d", str_len("apple"));
    // printf("\nlen: %d", str_len("one two three"));
    // printf("len: %d", str_len("apple"));

    // char buffer[50];

    // fputs("\nEnter stuff: ", stdout);
    // fgets(buffer, sizeof(buffer), stdin);
    // printf("# of tokens: %d", count_tokens(buffer));

    // char **tokens = tokenize(buffer);
    // print_tokens(tokens);
    // free_tokens(tokens);
    // free(tokens);
    // tokens = NULL;

    // char *s = token_start("foy toy");
    // printf("\nstart addr: %p char val: %c", s, *s);
    // s = token_terminator(s);
    // --s;

    // printf("\nend addr: %p char val: %c", s, *s);
    return 0;
}
