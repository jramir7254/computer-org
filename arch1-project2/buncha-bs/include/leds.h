#include <msp430.h>

#ifndef LEDS_H
#define LEDS_H

#define GREEN_LED BIT0
#define RED_LED BIT6
#define LEDS (GREEN_LED | RED_LED)

typedef enum
{
    BRIGHT_GREEN_LED,
    BRIGHT_RED_LED,
    DIM_GREEN_LED,
    DIM_RED_LED,
    NONE
} Active_Led;

typedef enum
{
    GREEN,
    RED,
} Led_Color;

typedef enum
{
    HIGH,
    LOW,
} Led_Brightness;

extern int active_led;

void leds_init();
void dim_red_led();
void dim_green_led();
void bright_green_led();
void bright_red_led();
void reset_leds();
void led_tick();
#endif
