#include "leds.h"

void leds_init()
{
	P1DIR |= LEDS;
	P1OUT &= ~LEDS;
	P1OUT |= RED_LED;
}

// global state vars that control blinking
int blinkLimit = 8;  // duty cycle = 1/blinkLimit
int blinkCount = 0;  // cycles 0...blinkLimit-1
int active_led = NONE;
void dim_red_led()
{
	blinkCount ++;
	if (blinkCount >= blinkLimit) { // on for 1 interrupt period
		blinkCount = 0;
		P1OUT |= RED_LED;
	} else		          // off for blinkLimit - 1 interrupt periods
    P1OUT &= ~RED_LED;
}


void dim_green_led()
 {
      blinkCount ++;
      if (blinkCount >= blinkLimit) { // on for 1 interrupt period
          blinkCount = 0;
         P1OUT |= GREEN_LED;
     } else                // off for blinkLimit - 1 interrupt periods
     P1OUT &= ~GREEN_LED;
}

void bright_red_led(){
	P1OUT |= RED_LED;
}

void bright_green_led(){
	P1OUT |= GREEN_LED;
}

void clear_leds()
{
	active_led = NONE;
	P1OUT &= ~LEDS;
}

void led_tick()
{
	static int ticks = 0;
	if(ticks++ >= 250) {
		P1OUT ^= RED_LED;
		P1OUT ^= GREEN_LED;
		ticks = 0;
	}
}
