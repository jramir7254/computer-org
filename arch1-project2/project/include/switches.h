#include <msp430.h>

#ifndef SWITCHES_H
#define SWITCHES_H

#define SWITCH_0 BIT3

#define SWITCH_1 BIT0
#define SWITCH_2 BIT1
#define SWITCH_3 BIT2
#define SWITCH_4 BIT3

#define PORT_1_SWITCHES SWITCH_0
#define PORT_2_SWITCHES (SWITCH_1 | SWITCH_2 | SWITCH_3 | SWITCH_4)

void switches_init();
char port_1_update_interrupt_sense();
char port_2_update_interrupt_sense();
void port_1_interrupt_handler();
void port_2_interrupt_handler();

void __interrupt_vec(PORT2_VECTOR) Port_1();
void __interrupt_vec(PORT2_VECTOR) Port_2();

#endif
