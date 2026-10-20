#ifndef EVENTS_H
#define EVENTS_H

extern volatile int last_button_pressed;

void port_1_button_pressed();

void port_2_button_pressed(int button);
void port_2_button_released();

#endif