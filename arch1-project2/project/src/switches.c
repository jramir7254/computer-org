#include <msp430.h>
#include "../include/switches.h"
#include "../include/events.h"

#define SWITCH_0 BIT3

#define SWITCH_1 BIT0
#define SWITCH_2 BIT1
#define SWITCH_3 BIT2
#define SWITCH_4 BIT3

#define PORT_1_SWITCHES SWITCH_0
#define PORT_2_SWITCHES (SWITCH_1 | SWITCH_2 | SWITCH_3 | SWITCH_4)

static char port_2_update_interrupt_sense()
{
    char p2val = P2IN;
    P2IES |= (p2val & PORT_2_SWITCHES);
    P2IES &= (p2val | ~PORT_2_SWITCHES);
    return p2val;
}

static char port_1_update_interrupt_sense()
{
    char p1val = P1IN;
    P1IES |= (p1val & PORT_1_SWITCHES);
    P1IES &= (p1val | ~PORT_1_SWITCHES);
    return p1val;
}

static void port_1_interrupt_handler()
{
    char p1val = port_1_update_interrupt_sense();
    char switches = ~p1val & PORT_1_SWITCHES;

    if (switches & SWITCH_0)
        port_1_button_pressed();
}

static void port_2_interrupt_handler()
{
    char p2val = port_2_update_interrupt_sense();
    char switches = ~p2val & PORT_2_SWITCHES;

    if (switches & SWITCH_1)
        port_2_button_pressed(0);
    else if (switches & SWITCH_2)
        port_2_button_pressed(1);
    else if (switches & SWITCH_3)
        port_2_button_pressed(2);
    else if (switches & SWITCH_4)
        port_2_button_pressed(3);
    else
        port_2_button_released();
}

void switches_init()
{
    P1DIR &= ~PORT_1_SWITCHES;
    P1OUT |= PORT_1_SWITCHES;
    P1REN |= PORT_1_SWITCHES;
    P1IE |= PORT_1_SWITCHES;

    P2DIR &= ~PORT_2_SWITCHES;
    P2REN |= PORT_2_SWITCHES;
    P2OUT |= PORT_2_SWITCHES;
    P2IE |= PORT_2_SWITCHES;

    port_1_update_interrupt_sense();
    port_2_update_interrupt_sense();
}

void __interrupt_vec(PORT1_VECTOR) Port_1()
{
    if (P1IFG & PORT_1_SWITCHES)
    {
        P1IFG &= ~PORT_1_SWITCHES;
        port_1_interrupt_handler();
    }
}

void __interrupt_vec(PORT2_VECTOR) Port_2()
{
    if (P2IFG & PORT_2_SWITCHES)
    {
        P2IFG &= ~PORT_2_SWITCHES;
        port_2_interrupt_handler();
    }
}
