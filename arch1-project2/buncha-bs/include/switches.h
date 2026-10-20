#ifndef SWITCHES_H
#define SWITCHES_H

#include "../include/leds.h"
#include "../include/pitches.h"
// static char switch_update_interrupt_sense();

typedef struct Switch
{

    Led_Color color;
    Led_Brightness brightness;
} Switch;

void switch_init();

void switch_interrupt_handler();

extern volatile int game_started;

void __interrupt_vec(PORT2_VECTOR) Port_2();
#endif
