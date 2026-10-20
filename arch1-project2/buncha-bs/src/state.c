#include "../include/state.h"

volatile int current_state = GAME_START;

volatile int seed = 0b0001;
volatile int lives = 3;
volatile int game_seed = 0b0001;
volatile int curr_seed = 0b0001;

volatile int total_len = 1;
volatile int curr_len = 0;

int lfsr()
{
    int bit3 = (curr_seed >> 3) & 1;
    int bit2 = (curr_seed >> 2) & 1;
    int xor = bit3 ^ bit2;
    curr_seed = (curr_seed << 1) | xor;
    return curr_seed & 3;
}

void handle_button_click()
{
    if (current_state != PLAYER_TURN)
        return;
}

void update_state(State next_state)
{
}

void reset_hardware()
{
    reset_leds();
    buzzer_stop();
}

void state_update()
{
    switch (current_state)
    {
    case GAME_LOAD:
        /* code */
        break;

    default:
        break;
    }
}

void seed_generator()
{

    static int seed_tick = 1;
    seed = seed_tick;

    if (seed > MAX_SEED)
    {
        seed_tick = 1;
    }
}
