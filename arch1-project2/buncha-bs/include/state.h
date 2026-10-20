#ifndef STATE_H
#define STATE_H

typedef enum
{
    GAME_LOAD,
    GAME_START,
    COMPUTER_TURN,
    PLAYER_TURN,
    RIGHT_ANSWER,
    WRONG_ANSWER,
    TRANSITION,
    GAME_OVER
} State;

// #define MAX_SEED 65535
#define MAX_SEED 15
extern volatile int seed;

extern volatile int current_state;
extern volatile int game_seed;
extern volatile int curr_seed;
extern volatile int lives;

extern volatile int total_len;
extern volatile int curr_len;

void seed_generator();
int lfsr();

#endif