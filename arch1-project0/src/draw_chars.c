#include "draw.h"  /* for font */
#include "stdio.h" /* for putchar */
#include <stdlib.h>
#include <time.h>

const char bright_neon_colors[][12] = {
    BRIGHT_CYAN_TEXT,
    BRIGHT_MAGENTA_TEXT,
    BRIGHT_YELLOW_TEXT,
};

const char neon_colors[][12] = {
    DIM CYAN_TEXT,
    DIM MAGENTA_TEXT,
    DIM YELLOW_TEXT,
};

void print_char_5x7(char c)
{
    c -= 0x20;
    for (char col = 0; col < 5; col++)
    {
        for (char row = 0; row < 7; row++)
        {
            unsigned short rowBits = font_5x7[c][col];
            unsigned short colMask = 1 << (6 - row); /* mask to select bit associated with bit */
            putchar((rowBits & colMask) ? '*' : ' ');
        }
        putchar('\n');
    }
    putchar('\n');
}

void print_neon_char_8x12(char c, int bright)
{
    c -= 0x20;
    for (char row = 0; row < 12; row++)
    {
        for (char col = 0; col < 8; col++)
        {

            unsigned short rowBits = font_8x12[c][row];
            unsigned short colMask = 1 << (7 - col);

            printf("%s%c" RESET,
                   bright ? bright_neon_colors[c % 3] : neon_colors[c % 3],
                   !(rowBits & colMask) ? ' ' : bright ? c
                                                       : c + 0x20);
        }
        putchar('\n');
    }
    putchar('\n');
}
