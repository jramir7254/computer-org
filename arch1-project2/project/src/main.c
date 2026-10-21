#include <msp430.h>
#include "libTimer.h"
#include "../include/buzzer.h"
#include "../include/switches.h"
#include "../include/state.h"
#include "../include/leds.h"

int main()
{
    configureClocks();
    enableWDTInterrupts();
    buzzer_init();
    leds_init();
    switches_init();
    or_sr(0x18);
}

void __interrupt_vec(WDT_VECTOR) WDT() /* ~250 times per second */
{
    state_machine();
    leds_tick();
}
