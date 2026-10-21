#include "../include/events.h"
#include "../include/game.h"
#include "../include/state.h"
#include "../include/actions.h"

int last_button_pressed = -1;

void port_1_button_pressed()
{
    if (current_game_state != GAME_LOAD)
        return;

    update_state(COMPUTER_TURN);
}

void port_2_button_pressed(int button)
{
    if (current_game_state != PLAYER_TURN)
        return;

    last_button_pressed = button;

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
    }
}

void port_2_button_released()
{
    if (current_game_state != PLAYER_TURN || last_button_pressed == -1)
        return;

    reset_hardware();
    check_sequence(last_button_pressed);
    last_button_pressed = -1;
}