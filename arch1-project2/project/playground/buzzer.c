#include <msp430.h>
#include "libTimer.h"
#include "buzzer.h"

char buzzer_state = 0;

/* ---------------- Basic buzzer ---------------- */

void buzzer_init()
{
  /* Send Timer A output TA0.1 to P2.6 (datasheet table 21):
     P2SEL2.6, P2SEL2.7 and P2SEL.7 must be 0, P2SEL.6 must be 1 */
  timerAUpmode();
  P2SEL2 &= ~(BIT6 | BIT7);
  P2SEL  &= ~BIT7;
  P2SEL  |=  BIT6;
  P2DIR  |=  BIT6;        /* P2.6 output (|= so other Port 2 pins are left alone) */
}

void buzzer_set_period(unsigned short cycles)   /* buzzer clock = 2 MHz */
{
  CCR0 = cycles;
  CCR1 = cycles >> 1;     /* switch halfway through: a clean square wave */
}

/* ---------------- Phrases ---------------- */
/* Bigger period = lower pitch.  2,000,000 / period = Hz.  Keep periods <= 65535. */

//const Sound hello[] = {
 // {4000, 3600, 25},       /* "heh": short, slight rise (500 -> 555 Hz) */
 // {0,    0,     4},       /* gap */
 // {3300, 4400, 55},       /* "lo": falls (600 -> 450 Hz) */
 // {0,    0,     0},       /* end */
//};

const Sound hello[] = {
  { 5115,  4728,  25},
  { 4728,  4405,   5},
  { 4405,  4629,  35},
  { 4629,  4854,   5},
  { 4854,  5063,  15},
  { 5063,  5221,   5},
  { 5221,  5390,   5},
  { 5390,  5555,   5},
  { 5555,  5747,   5},
  { 5747,  5747,  20},
  {    0,     0,  30},
  {    0,     0,   0},
};
const Sound uh_oh[] = {
  {3333, 3333, 35},       /* "uh": steady ~600 Hz */
  {0,    0,     8},       /* gap */
  {4000, 5000, 60},       /* "oh": slides down 500 -> 400 Hz */
  {0,    0,     0},
};

const Sound huh[] = {
  {5000, 3000, 50},       /* rises 400 -> 667 Hz, like a question */
  {0,    0,     0},
};

/* ---------------- Phrase player ---------------- */

static const Sound *phrase = 0;   /* what's playing (0 = nothing) */
static unsigned char step = 0;    /* which step of the phrase */
static unsigned char t = 0;       /* ticks into the current step */

void say(const Sound *p)
{
  phrase = p;
  step = 0;
  t = 0;
  if (!p)
    buzzer_set_period(0);         /* say(0) stops the sound */
}

char is_talking()
{
  return phrase != 0;
}

void speech_tick()
{
  if (!phrase)
    return;

  const Sound *s = &phrase[step];
  if (s->ticks == 0) {            /* reached the end marker */
    buzzer_set_period(0);
    phrase = 0;
    return;
  }

  /* slide from start to end across the step */
  long p = s->start + ((long)s->end - (long)s->start) * t / s->ticks;
  buzzer_set_period((unsigned short)p);

  if (++t >= s->ticks) {          /* step done: move to the next one */
    t = 0;
    step++;
  }
}
