#include "../include/game.h"
#include "../include/rng.h"
#include "../include/state.h"

volatile int game_seed;
volatile int current_seed;

volatile int player_lives = 3;

volatile int total_sequence_length = 1;
volatile int current_sequence_step = 0;

void restart_sequence()
{
    current_sequence_step = 0;
    current_seed = game_seed;
}

static void increment_sequence_length()
{
    total_sequence_length++;
}

static void reset_game()
{
    restart_sequence();
    player_lives = 3;
    total_sequence_length = 1;
    update_state(GAME_OVER);
}

static int is_game_over()
{
    return player_lives <= 0;
}

void check_sequence(int button)
{

    int next_sequence_step = lfsr();

    if (next_sequence_step != button)
    {
        player_lives--;
        if (is_game_over())
        {
            reset_game();
        }
        else
        {
            restart_sequence();
            update_state(WRONG_ANSWER);
        }
        return;
    }

    if (++current_sequence_step >= total_sequence_length)
    {
        restart_sequence();
        increment_sequence_length();
        update_state(RIGHT_ANSWER);
    }
}
