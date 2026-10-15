#include <msp430.h>
#include "../include/buzzer.h"
#include "../include/leds.h"
#define SW1 BIT0
#define SW2 BIT1
#define SW3 BIT2
#define SW4 BIT3

#define SWITCHES (SW1 | SW2 | SW3 | SW4)


static char switch_update_interrupt_sense()
{
  char p2val = P2IN;
  /* update switch interrupt to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);	/* if switch up, sense down */
  P2IES &= (p2val | ~SWITCHES);	/* if switch down, sense up */
  return p2val;
}
int switches = 0;

void switch_init()
{
	P2DIR &= ~SWITCHES;
	P2REN |=  SWITCHES;
	P2OUT |=  SWITCHES;
	P2IE  |=  SWITCHES;
	switch_update_interrupt_sense();
}


void switch_interrupt_handler()
{
	char p2val = switch_update_interrupt_sense();
	switches = ~p2val & SWITCHES;
	
	if (switches & SW1)
       // say(hello);
        active_led = BRIGHT_GREEN_LED;                    // stop any phrase so the tick doesn't overwrite the note
    else if (switches & SW2)
        //say(uh_oh);
        active_led = DIM_GREEN_LED;                    // stop any phrase so the tick doesn't overwrite the note
    else if (switches & SW3)
        //say(huh);
        active_led = BRIGHT_RED_LED;                    // stop any phrase so the tick doesn't overwrite the note
    else if (switches & SW4) {
        active_led = DIM_RED_LED;                    // stop any phrase so the tick doesn't overwrite the note
        //buzzer_set_period(5102);   // hold a steady G
    }
	else {

		active_led = NONE;
		if(!is_talking())
			buzzer_set_period(0);
	}		
}

void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {	      /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;		      /* clear pending sw interrupts */
    switch_interrupt_handler();	/* single handler for all switches */
  }
}
