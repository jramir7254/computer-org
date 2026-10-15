#include <msp430.h>
#include "libTimer.h"
#include "buzzer.h"
#include "switches.h"
#include "leds.h"


int main()
{
    configureClocks();
	enableWDTInterrupts(); 
	buzzer_init();
	leds_init();
	switch_init();
	or_sr(0x18);  
}
void __interrupt_vec(WDT_VECTOR) WDT()   /* ~250 times per second */
{
//	led_tick();
	if(active_led == BRIGHT_RED_LED){
		bright_red_led();
	}
	else if(active_led == DIM_RED_LED){
		dim_red_led();
	}
	else if(active_led == BRIGHT_GREEN_LED){
		bright_green_led();
	}
	else if(active_led == DIM_GREEN_LED){
		dim_green_led();
	}
	else {
		clear_leds();
	}
	speech_tick();
}
