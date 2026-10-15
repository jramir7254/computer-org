#ifndef SWITCHES_H
#define SWITCHES_H
//static char switch_update_interrupt_sense();

void switch_init();

void switch_interrupt_handler();

void __interrupt_vec(PORT2_VECTOR) Port_2();
#endif
