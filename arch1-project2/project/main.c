#include <msp430.h>
#include "libTimer.h"
#include "buzzer.h"
#include "pitches.h"


static int melody[] = {
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4, 
  NOTE_AS3, NOTE_AS4, 0,
  0,
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4, 
  NOTE_AS3, NOTE_AS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0, NOTE_DS4, NOTE_CS4, NOTE_D4,
  NOTE_CS4, NOTE_DS4, 
  NOTE_DS4, NOTE_GS3,
  NOTE_G3, NOTE_CS4,
  NOTE_C4, NOTE_FS4,NOTE_F4, NOTE_E3, NOTE_AS4, NOTE_A4,
  NOTE_GS4, NOTE_DS4, NOTE_B3,
  NOTE_AS3, NOTE_A3, NOTE_GS3,
  0, 0, 0, -1
};
static int tempo[] = {
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

static unsigned short quarter_duration = 25;
static unsigned short current_note = 0;
static unsigned short duration = 0;


int main() {
    configureClocks();
 
    buzzer_init();
    duration = (int)(quarter_duration);
    current_note = 0;



int note = melody[current_note];

	while(note >= 0) {

	  // Get the current note from the song
	  int note = melody[current_note];
	  
	  // Check if the song has ended
	  if(note < 0){
	  }


	  // Play the current note
	  buzzer_set_period(note);
	  // Update the note duration
	  duration--;

	  // We finished playing the note
	  if(duration <= 1){
	    // Go to the next note
	    current_note++;
	    // Update the note duration
	    duration = (int)(quarter_duration);
	  }  
	  
	}
}
