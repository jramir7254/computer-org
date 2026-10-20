#include <msp430.h>
#include "../include/buzzer.h"
#include "../include/leds.h"
#include "../include/pitches.h"
#include "../include/switches.h"
#include "../include/state.h"

#define SW1 BIT0
#define SW2 BIT1
#define SW3 BIT2
#define SW4 BIT3

#define SW0 BIT3

#define SWITCHES (SW1 | SW2 | SW3 | SW4)

static char switch_update_interrupt_sense()
{
    char p2val = P2IN;
    P2IES |= (p2val & SWITCHES);  /* if switch up, sense down */
    P2IES &= (p2val | ~SWITCHES); /* if switch down, sense up */
    return p2val;
}

int switches = 0;

void switch_init()
{
    P1DIR &= ~SW0;
    P1REN |= SW0;
    P1OUT |= SW0;
    P1IE |= SW0;

    P2DIR &= ~SWITCHES;
    P2REN |= SWITCHES;
    P2OUT |= SWITCHES;
    P2IE |= SWITCHES;
    switch_update_interrupt_sense();
}

int led_1;

int a = -1;

void switch_interrupt_handler()
{

    char p2val = switch_update_interrupt_sense();
    switches = ~p2val & SWITCHES;

    reset_leds();
    buzzer_stop();

    if (curr_len >= total_len)
    {
        a = 1;
    }

    // int led_1 = seed_here();

    if ((switches & SW1))
    {
        buzzer_set_period(NOTE_C4);
        active_led = BRIGHT_GREEN_LED; // s
        led_1 = lfsr();
        if (led_1 != 0)
        {
            curr_len = 0;
            a = 0;
            curr_seed = game_seed;
        }
        else
        {

            curr_len++;
        }
    }
    else if ((switches & SW2))
    {
        buzzer_set_period(NOTE_E4);
        active_led = DIM_GREEN_LED;
        led_1 = lfsr();

        if (led_1 != 1)
        {
            curr_len = 0;
            a = 0;

            curr_seed = game_seed;
        }
        else
        {

            curr_len++;
        }

        // stop any phrase so the tick doesn't overwrite the note
    }
    else if ((switches & SW3))
    {
        buzzer_set_period(NOTE_G4);
        active_led = BRIGHT_RED_LED; // stop any phrase so the tick doesn't overwrite the note
        led_1 = lfsr();

        if (led_1 != 2)
        {
            curr_len = 0;
            a = 0;

            curr_seed = game_seed;
        }
        else
        {

            curr_len++;
        }
    }
    else if ((switches & SW4))
    {
        buzzer_set_period(NOTE_B4);
        active_led = DIM_RED_LED;
        led_1 = lfsr();

        if (led_1 != 3)
        {
            curr_len = 0;
            a = 0;

            curr_seed = game_seed;
        }
        else
        {

            curr_len++;
        }
    }

    else
    {
        if (a == 0)
        {
            lives--;
            if (lives <= 0)
            {
                current_state = GAME_OVER;
                curr_sound = OVER;
                return;
            }
            curr_seed = game_seed;
            curr_len = 0;
            current_state = TRANSITION;
            curr_sound = WRONG;

            buzzer_set_period(0);
            a = -1;
            return;
        }
        else if (a == 1)
        {
            curr_seed = game_seed;
            curr_len = 0;
            total_len++;
            curr_sound = CORRECT;
            current_state = TRANSITION;
            a = -1;
            buzzer_set_period(0);
            return;
        }
        active_led = NONE;
    }
}

void __interrupt_vec(PORT1_VECTOR) Port_1()

{
    if (current_state != GAME_LOAD)
        return;

    if (P1IFG & SW0)
    {                  /* did a button cause this interrupt? */
        P1IFG &= ~SW0; /* clear pending sw interrupts */
        char p1val = P1IN;
        P1IES |= (p1val & SW0);  /* if switch up, sense down */
        P1IES &= (p1val | ~SW0); /* if switch down, sense up */

        char switch0 = ~p1val & SW0;
        if ((switch0 & SW0))
        {
            current_state = GAME_START;
            return;
        }
    }
}

void __interrupt_vec(PORT2_VECTOR) Port_2()
{
    if (current_state != PLAYER_TURN)
        return;

    if (P2IFG & SWITCHES)
    {                               /* did a button cause this interrupt? */
        P2IFG &= ~SWITCHES;         /* clear pending sw interrupts */
        switch_interrupt_handler(); /* single handler for all switches */
    }
}
