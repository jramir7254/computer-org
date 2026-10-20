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

extern State current_game_state;
extern State next_game_state;

void computer_turn();
void transition_state();
void state_machine();
void update_state(State next_state);

#endif