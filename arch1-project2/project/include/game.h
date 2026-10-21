#ifndef GAME_H
#define GAME_H

extern volatile int game_seed;
extern volatile int current_seed;

extern volatile int player_lives;

extern volatile int total_sequence_length;
extern volatile int current_sequence_step;

void restart_sequence();
void check_sequence(int button);

#endif