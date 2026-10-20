#ifndef buzzer_included
#define buzzer_included

typedef enum
{
    CORRECT,
    WRONG,
    START,
    SILENCE,
    OVER,
} Sounds;

extern int curr_sound;

/* ---- Basic buzzer ---- */
void buzzer_init();
void buzzer_stop();
void buzzer_set_period(unsigned short cycles); /* 2,000,000 / cycles = Hz; 0 = silent */
void play_sound(const int *melody, const int *tempo);
void play_start();
void play_right();
void play_wrong();
void play_over();

#endif // included
