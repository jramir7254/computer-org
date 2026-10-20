#include "../include/leds.h"

void leds_init()
{
    P1OUT &= ~LEDS;
    P1DIR |= LEDS;
}

Color active_led_color;
Brightness active_led_brightness;

void led_on(Color color)
{
    P1OUT |= color == RED ? RED_LED : GREEN_LED;
}

void led_dim()
{
    static int blinkLimit = 25;
    static int blinkCount = 0;

    if (blinkCount++ >= blinkLimit)
    {
        blinkCount = 0;
        P1OUT |= active_led_color == RED ? RED_LED : GREEN_LED;
    }
    else
        P1OUT &= ~(active_led_color == RED ? RED_LED : GREEN_LED);
}

void leds_off()
{
    active_led_brightness = OFF;
    P1OUT &= ~LEDS;
}

void leds_set_active(Color color, Brightness brightness)
{
    active_led_color = color;
    active_led_brightness = brightness;
    led_on(color);
}

void leds_tick()
{
    if (active_led_brightness == LOW)
        led_dim();

    // switch (active_led)
    // {
    // case BRIGHT_RED_LED:
    //     bright_red_led();
    //     break;
    // case DIM_RED_LED:
    //     dim_red_led();
    //     break;
    // case BRIGHT_GREEN_LED:
    //     bright_green_led();
    //     break;
    // case DIM_GREEN_LED:
    //     dim_green_led();
    //     break;
    // default:
    //     leds_off();
    //     break;
    // }
}
