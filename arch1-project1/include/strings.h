#ifndef STRINGS_H
#define STRINGS_H

int str_equals(char *str, const char *comp);

int str_starts_with(char *str, char comp);

void str_remove_newln(char *str);

short str_len(char *str);

short token_len(char *str);

#endif