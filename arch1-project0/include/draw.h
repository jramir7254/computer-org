#ifndef DRAW_INCLUDED
#define DRAW_INCLUDED

#define DIM "\033[2m"

#define RESET "\033[0m"

#define YELLOW_TEXT "\033[0;33m"
#define MAGENTA_TEXT "\033[0;35m"
#define CYAN_TEXT "\033[0;36m"

#define BRIGHT_YELLOW_TEXT "\033[93m"
#define BRIGHT_MAGENTA_TEXT "\033[95m"
#define BRIGHT_CYAN_TEXT "\033[1;96m"

#define BRIGHT_GREEN_TEXT "\033[92m"

void print_triangle(int startCol, int size);
void print_square(int startCol, int size);
void print_arrow();
void print_char_5x7(char c);
void print_char_8x12(char c);
void print_neon_char_8x12(char c, int bright);

extern const unsigned char font_5x7[][5];
extern const unsigned char font_8x12[][12];

#endif
