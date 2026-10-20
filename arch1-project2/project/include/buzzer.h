#ifndef BUZZER_H
#define BUZZER_H

extern volatile int finished;

void buzzer_init();
void buzzer_stop();
void buzzer_set_period(unsigned short cycles);
void buzzer_play(const int *melody, const int *tempo);
void buzzer_reset();
void play_start_sound();
void play_right_answer_sound();
void play_wrong_answer_sound();
void play_game_over_sound();

#endif
