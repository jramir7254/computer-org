#include <msp430.h>
#include "libTimer.h"
#include "buzzer.h"
#include "pitches.h"
#include <stdint.h>

int main() {
    configureClocks();
	enableWDTInterrupts(); 
    buzzer_init();
   // buzzer_set_period(1000);	/* start buzzing!!! 2MHz/1000 = 2kHz*/


    or_sr(0x18);          // CPU off, GIE on
}
const int melody_1[] = {
  /* "one": rises a little, then falls */
  NOTE_E5, NOTE_G5, NOTE_F5, NOTE_D5,
  NOTE_REST,
  /* "two": quick "t" click, then "oo" falls from a bit higher */
  NOTE_C7, NOTE_A5, NOTE_G5, NOTE_E5,
  NOTE_REST,
  /* "three": "th" hiss, then "ree" starts high and falls the furthest */
  NOTE_B7, NOTE_A7, NOTE_B5, NOTE_A5, NOTE_F5, NOTE_C5,
 NOTE_REST,
  -1
};



const int tempo_1[] = {
  25, 12, 12, 12,       /* "one"   */
  3,                    /* pause   */
  50, 12, 12, 12,       /* "two"   */
  3,                    /* pause   */
  50, 50, 12, 12, 12, 8,/* "three" */
  2                     /* final pause */
};

const int melody[] = {
	NOTE_REST, NOTE_GS4, NOTE_A4, NOTE_GS4,		NOTE_G4, NOTE_GS4, NOTE_CS5, NOTE_E5,	NOTE_DS5, NOTE_CS5, NOTE_DS5, NOTE_CS5,		NOTE_C5, NOTE_CS5, NOTE_E5, NOTE_GS5,	NOTE_REST,
	NOTE_REST, NOTE_GS4, NOTE_A4, NOTE_GS4,		NOTE_G4, NOTE_GS4, NOTE_CS5, NOTE_E5,	NOTE_DS5, NOTE_CS5, NOTE_DS5, NOTE_CS5,		NOTE_C5, NOTE_CS5, NOTE_E5, NOTE_GS5,	NOTE_REST,
	NOTE_REST, NOTE_A4, NOTE_CS5, NOTE_DS5,		NOTE_FS5, NOTE_A5, NOTE_CS6, NOTE_DS6,	NOTE_B6, NOTE_A6, NOTE_GS6, NOTE_FS6,		NOTE_E6, NOTE_DS6, NOTE_FS6, NOTE_CS6,	NOTE_REST,
	NOTE_C6, NOTE_DS6, NOTE_A5, NOTE_GS5,		NOTE_FS5, NOTE_A5, NOTE_E5, NOTE_DS5, 	NOTE_FS5, NOTE_CS5, NOTE_C5, NOTE_DS5,		NOTE_A4, NOTE_GS4, NOTE_B4, NOTE_A4,	NOTE_REST,
	NOTE_REST, NOTE_GS4, NOTE_A4, NOTE_GS4,		NOTE_G4, NOTE_GS4, NOTE_CS5, NOTE_E5,	NOTE_DS5, NOTE_CS5, NOTE_DS5, NOTE_CS5,		NOTE_C5, NOTE_CS5, NOTE_E5, NOTE_GS5,	NOTE_REST,
	NOTE_REST, NOTE_GS4, NOTE_AS4, NOTE_GS4,	NOTE_G4, NOTE_GS4, NOTE_CS5, NOTE_E5,	NOTE_DS5, NOTE_CS5, NOTE_DS5, NOTE_CS5,		NOTE_C5, NOTE_CS5, NOTE_E5, NOTE_GS5,	NOTE_REST,
	NOTE_DS5, NOTE_E5, NOTE_DS5, NOTE_D5,		NOTE_DS5, NOTE_B5, NOTE_AS5, NOTE_GS5,	NOTE_G5, NOTE_E6, NOTE_DS6, NOTE_CS6,		NOTE_B5, NOTE_A5, NOTE_GS5, NOTE_FS5,	NOTE_REST,
	-1
};

const int tempo[] = {
  12, 12, 12, 12, 
  12, 12, 6,
  3,
  12, 12, 12, 12, 
  12, 12, 6,
  3,
  12, 12, 12, 12, 
  12, 12, 6,
  3,
  12, 12, 12, 12, 
  12, 12, 6,
  6, 18, 18, 18,
  6, 6,
  6, 6,
  6, 6,
  18, 18, 18,18, 18, 18,
  10, 10, 10,
  10, 10, 10,
  3, 3, 3
  };

static unsigned short quarter_duration = 250;
static unsigned short current_note = 0;
static unsigned short duration = 0;

const int hello_melody[] = {
  391, 423, 454, 432, 412, 395, 383, 371, 360, 348, 0, -1
};
const int hello_tempo[] = {
  10, 42, 7, 42, 16, 42, 42, 42, 42, 12, 8
};


 void
__interrupt_vec(WDT_VECTOR) WDT()
{ 
	int note = hello_melody[current_note];

  if (note < 0) {                       // song is over
    buzzer_set_period(0);
    return;                             // (to loop instead: current_note = 0; return;)
  }

  if (duration == 0) {                  // starting a new note
    duration = quarter_duration /  hello_tempo[current_note];
   // duration = quarter_duration /  5;
   // duration = quarter_duration /  (tempo_1[current_note] / 2);
    //duration = quarter_duration / (note == 0 ? 10 : 5); // tempo[current_note];
    if (note == 0)
      buzzer_set_period(0);             // rest
    else
      buzzer_set_period(2000000L / note);   // Hz → period
  }

 // if (duration == 2)                    // tiny gap at the end of each note,
//    buzzer_set_period(0);               //   so repeated notes don't blur together

  if (--duration == 0)                  // note finished → move on
    current_note++;
} 
