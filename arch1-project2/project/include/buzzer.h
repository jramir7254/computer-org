#ifndef buzzer_included
#define buzzer_included

/* ---- Basic buzzer ---- */
void buzzer_init();
void buzzer_set_period(unsigned short cycles);  /* 2,000,000 / cycles = Hz; 0 = silent */

extern char buzzer_state;

/* ---- "Talking" sounds ---- */

/* One step of a phrase: slide from start to end period over some ticks */
typedef struct {
  unsigned short start;   /* period at the start of the step (0 = silence) */
  unsigned short end;     /* period at the end (same as start = steady note) */
  unsigned char  ticks;   /* how long, in watchdog ticks (~4 ms each); 0 = end of phrase */
} Sound;

void say(const Sound *phrase);   /* start playing a phrase (0 = stop) */
void speech_tick();              /* call once per tick from the WDT handler */
char is_talking();               /* 1 while a phrase is playing */

/* Ready-made phrases (defined in buzzer.c) */
extern const Sound hello[];
extern const Sound uh_oh[];
extern const Sound huh[];

#endif // included
