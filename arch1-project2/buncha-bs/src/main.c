#include <msp430.h>
#include <stdio.h>
#include "libTimer.h"
#include "../include/buzzer.h"
#include "../include/switches.h"
#include "../include/state.h"
#include "../include/leds.h"
#include "pitches.h"

#define BUZZER 1

int main()
{
    configureClocks();
    enableWDTInterrupts();
    buzzer_init();
    leds_init();
    switch_init();
    or_sr(0x18);
}

int seed_tick = 250;

int trans_tick = 0;

void __interrupt_vec(WDT_VECTOR) WDT() /* ~250 times per second */
{
    // start_music();

    if (current_state == GAME_LOAD)
    {
        play_start();
        seed_generator();
    }

    if (current_state == GAME_OVER)
    {
        curr_len = 0;
        total_len = 1;
        curr_seed = game_seed;
        lives = 3;
        current_state = TRANSITION;
    }

    if (current_state == TRANSITION)
    {
        if (trans_tick++ >= 250)
        {
            switch (curr_sound)
            {
            case CORRECT:
                play_right();
                break;
            case WRONG:
                play_wrong();
                break;
            case START:
                play_start();
                break;
            case OVER:
                play_over();
                break;
            default:
                current_state = COMPUTER_TURN;
            }
        }
    }
    else
    {
        trans_tick = 0;
        curr_sound = SILENCE;
    }

    if (current_state == COMPUTER_TURN && seed_tick++ >= 250)
    // if (seed_tick++ >= 250)
    {

        reset_leds();
        buzzer_stop();

        if (seed_tick < 350)
            return;

        seed_tick = 0;

        if (curr_len++ >= total_len)
        {
            current_state = PLAYER_TURN;
            curr_len = 0;
            seed_tick = 250;
            // total_len++;
            curr_seed = game_seed;
            return;
        }

        int led = lfsr();
        switch (led)
        {
        case 0:
            active_led = BRIGHT_GREEN_LED;
            BUZZER ? (buzzer_set_period(NOTE_C4)) : NULL;
            break;
        case 1:
            active_led = DIM_GREEN_LED;
            BUZZER ? (buzzer_set_period(NOTE_E4)) : NULL;
            break;
        case 2:
            active_led = BRIGHT_RED_LED;
            BUZZER ? (buzzer_set_period(NOTE_G4)) : NULL;
            break;
        case 3:
            active_led = DIM_RED_LED;
            BUZZER ? (buzzer_set_period(NOTE_B4)) : NULL;
            break;
        default:
            reset_leds();
        }
    }
    //	led_tick();
    if (active_led == BRIGHT_RED_LED)
    {
        bright_red_led();
    }
    else if (active_led == DIM_RED_LED)
    {
        dim_red_led();
    }
    else if (active_led == BRIGHT_GREEN_LED)
    {
        bright_green_led();
    }
    else if (active_led == DIM_GREEN_LED)
    {
        dim_green_led();
    }
    else
    {
        // active_led = NONE;
        reset_leds();
    }
}
