#include "../include/leds.h"
#include "../include/pitches.h"
#include "../include/buzzer.h"
#include "../include/actions.h"

static void toggle_action(Color led_color, Brightness led_brightness, unsigned short pitch)
{
    reset_hardware();
    leds_set_active(led_color, led_brightness);
    buzzer_set_period(pitch);
}

void step(int button)
{
    switch (button)
    {
    case 0:
        toggle_action_1();
        break;
    case 1:
        toggle_action_2();
        break;
    case 2:
        toggle_action_3();
        break;
    case 3:
        toggle_action_4();
        break;

    default:
        reset_hardware();
        break;
    }
}

void toggle_action_1()
{
    toggle_action(GREEN, HIGH, NOTE_C4);
}
void toggle_action_2()
{
    toggle_action(GREEN, LOW, NOTE_E4);
}
void toggle_action_3()
{
    toggle_action(RED, HIGH, NOTE_G4);
}
void toggle_action_4()
{
    toggle_action(RED, LOW, NOTE_B5);
}

void reset_hardware()
{
    leds_off();
    buzzer_reset();
}
