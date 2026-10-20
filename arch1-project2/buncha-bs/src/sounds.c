#include "../include/pitches.h"

const int game_init_melody[] = {
    NOTE_C5, NOTE_E5, NOTE_G5, NOTE_C6,
    REST,
    NOTE_A5, NOTE_B5,
    NOTE_C6,
    NOTE_E6, NOTE_G6,
    NOTE_C7,
    REST,
    STOP};

const int game_init_tempo[] = {
    16, 16, 16, 16,
    25,
    16, 16,
    6,
    25, 25,
    4,
    4};

const int game_over_melody[] = {
    NOTE_G5, NOTE_FS5, NOTE_F5,
    NOTE_E5, NOTE_DS5, NOTE_E5,
    NOTE_DS5, NOTE_E5, NOTE_DS5,
    REST,
    STOP};

const int game_over_tempo[] = {
    6, 6, 6,
    20, 20, 20,
    20, 20, 20,
    4};

const int right_answer_melody[] = {NOTE_C5, NOTE_G5, STOP};
const int right_answer_tempo[] = {16, 4};

const int wrong_answer_melody[] = {NOTE_C4, REST, NOTE_C4, STOP};
const int wrong_answer_tempo[] = {8, 8, 8};
