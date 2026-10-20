#include <msp430.h>
#include "libTimer.h"
#include "../include/buzzer.h"
#include "../include/pitches.h"
#include "../include/sounds.h"
#include "../include/state.h"

int curr_sound = START;
int finished = 0;

static unsigned short quarter_duration = 250;
static unsigned short current_note = 0;
static unsigned short duration = 0;

void buzzer_init()
{
    timerAUpmode();
    P2SEL2 &= ~(BIT6 | BIT7);
    P2SEL &= ~BIT7;
    P2SEL |= BIT6;
    P2DIR |= BIT6;
}

void buzzer_play(const int *melody, const int *tempo)
{
    int note = melody[current_note];

    if (note == STOP)
    {
        current_note = 0;
        duration = 0;
        finished = 1;
        // current_state = COMPUTER_TURN;
        buzzer_stop();
        return;
    }

    if (duration == 0)
    {
        duration = quarter_duration / tempo[current_note];
        if (note == REST)
            buzzer_stop();
        else
            buzzer_set_period(2000000L / note); // Hz → period
    }

    if (--duration == 0) // note finished → move on
        current_note++;
}

void play_right()
{
    buzzer_play(right_answer_melody, right_answer_tempo);
}

void play_wrong()
{
    buzzer_play(wrong_answer_melody, wrong_answer_tempo);
}

void play_start()
{
    if (finished)
        return;
    buzzer_play(game_init_melody, game_init_tempo);
}

void play_over()
{
    buzzer_play(game_over_melody, game_over_tempo);
}

void buzzer_reset()
{
    finished = 0;
    buzzer_stop();
}

void buzzer_stop()
{
    buzzer_set_period(0);
}

void buzzer_set_period(unsigned short cycles) /* buzzer clock = 2 MHz */
{
    CCR0 = cycles;
    CCR1 = cycles >> 1; /* switch halfway through: a clean square wave */
}
