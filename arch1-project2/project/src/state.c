#include "../include/state.h"
#include "../include/leds.h"
#include "../include/pitches.h"
#include "../include/actions.h"
#include "../include/rng.h"
#include "../include/buzzer.h"
#include "../include/game.h"

State current_game_state = GAME_LOAD;
State next_game_state;

void transition_state()
{
    static int transition_tick = 0;

    if (transition_tick++ >= 100)
    {
        current_game_state = next_game_state;
        transition_tick = 0;
    }
}

void computer_turn()
{
    static int computer_tick = 250;
    if (computer_tick++ >= 250)
    {

        reset_hardware();

        if (computer_tick < 350)
            return;

        computer_tick = 0;

        if (current_sequence_step++ >= total_sequence_length)
        {
            computer_tick = 250;
            restart_sequence();
            update_state(PLAYER_TURN);
            return;
        }

        int next_sequence_step = lfsr();
        step(next_sequence_step);
    }
}

void update_state(State next_state)
{
    reset_hardware();
    next_game_state = next_state;
    current_game_state = TRANSITION;
}

void state_machine()
{
    switch (current_game_state)
    {
    case GAME_LOAD:
        play_start_sound();
        seed_generator();
        break;
    case COMPUTER_TURN:
        computer_turn();
        break;
    case RIGHT_ANSWER:
        play_right_answer_sound();
        if (finished)
        {
            update_state(COMPUTER_TURN);
        }
        break;
    case WRONG_ANSWER:
        play_wrong_answer_sound();
        if (finished)
        {
            update_state(COMPUTER_TURN);
        }
        break;
    case GAME_OVER:
        play_game_over_sound();
        if (finished)
        {
            update_state(GAME_LOAD);
        }
        break;
    case TRANSITION:
        transition_state();
        break;
    default:
        break;
    }
}