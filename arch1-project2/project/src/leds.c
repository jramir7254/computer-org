#include <msp430.h>
#include "../include/leds.h"

#define GREEN_LED BIT0
#define RED_LED BIT6
#define LEDS (GREEN_LED | RED_LED)

Color active_led_color;
Brightness active_led_brightness;

void leds_init()
{
    P1OUT &= ~LEDS;
    P1DIR |= LEDS;
}

void led_on(Color color)
{
    P1OUT |= color == RED ? RED_LED : GREEN_LED;
}

void alternate_leds()
{
    static int alternate_tick = 0;

    if (alternate_tick++ <= 250)
    {
        leds_off();
        leds_set_active(GREEN, HIGH);
    }
    else if (alternate_tick < 500)
    {
        leds_off();
        leds_set_active(RED, HIGH);
    }
    else
    {
        alternate_tick = 0;
    }
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

void flash_red_led()
{
    leds_off();
    static int flash_tick = 0;

    if (flash_tick++ < 25)
    {
        leds_set_active(RED, HIGH);
    }
    else if (flash_tick < 50)
    {
        leds_off();
    }
    else
    {
        flash_tick = 0;
    }
}

void leds_tick()
{
    if (active_led_brightness == LOW)
        led_dim();
}
