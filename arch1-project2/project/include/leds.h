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

typedef enum Led_Color
{
    GREEN,
    RED,
} Color;

typedef enum Led_Brightness
{
    HIGH,
    LOW,
    OFF,
} Brightness;

extern Color active_led_color;
extern Brightness active_led_brightness;

void leds_set_active(Color color, Brightness brightness);
void led_dim();
void leds_off();
void leds_init();
void leds_tick();

#endif
