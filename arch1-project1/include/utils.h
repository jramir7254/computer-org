#ifndef UTILS_H
#define UTILS_H

/**
 * @brief Returns the length of a string/token.
 *
 * @param str
 * @return short
 */
short str_len(char *str);

short token_len(char *str);

int is_history_cmmd(char *str);

int is_quit_cmmd(char *str);

int is_clear_cmmd(char *str);

int is_retrieval_cmmd(char *str);

int is_char_num(char c);

/**
 * @brief Removes the trailing new line from user input.
 *
 * @param str
 */
void remove_newln(char *str);

int get_numeric_val(char *str);

#endif