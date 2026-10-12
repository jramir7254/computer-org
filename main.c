#include <stdio.h>

#define BIT0 (0x0001)
#define BIT1 (0x0002)
#define BIT2 (0x0004)
#define BIT3 (0x0008)
#define BIT4 (0x0010)
#define BIT5 (0x0020)
#define BIT6 (0x0040)
#define BIT7 (0x0080)
#define BIT8 (0x0100)
#define BIT9 (0x0200)
#define BITA (0x0400)
#define BITB (0x0800)
#define BITC (0x1000)
#define BITD (0x2000)
#define BITE (0x4000)
#define BITF (0x8000)

int main()
{

    printf("%d %d %d %d", 0b1, 0b10, 0b100, 0b0110);
    int b0 = 0b0001;
    int b6 = 0b0110;
    printf("\nBIT0: %d", BIT0);
    printf("\nBIT1: %d", BIT1);
    printf("\nBIT2: %d", BIT2);
    printf("\nBIT3: %d", BIT3);
    printf("\nBIT4: %d", BIT4);
    printf("\nBIT5: %d", BIT5);
    printf("\nBIT6: %d", BIT6);
    // printf("\n(BIT0 | BIT6): %d", (BIT0 | BIT6));
    return 0;
}