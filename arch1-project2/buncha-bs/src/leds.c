#include "leds.h"

void leds_init()
{
    P1OUT &= ~LEDS;
    P1DIR |= LEDS;
}

int blinkLimit = 25;
int blinkCount = 0;
int active_led = NONE;
void dim_red_led()
{
    blinkCount++;
    if (blinkCount >= blinkLimit)
    {
        blinkCount = 0;
        P1OUT |= RED_LED;
    }
    else
        P1OUT &= ~RED_LED;
}

void dim_green_led()
{
    blinkCount++;
    if (blinkCount >= blinkLimit)
    {
        blinkCount = 0;
        P1OUT |= GREEN_LED;
    }
    else
        P1OUT &= ~GREEN_LED;
}

void bright_red_led()
{
    P1OUT |= RED_LED;
}

void bright_green_led()
{
    P1OUT |= GREEN_LED;
}

void reset_leds()
{
    active_led = NONE;
    P1OUT &= ~LEDS;
}

void led_tick()
{
    static int ticks = 0;
    if (ticks++ >= 250)
    {
        P1OUT ^= RED_LED;
        P1OUT ^= GREEN_LED;
        ticks = 0;
    }
}
