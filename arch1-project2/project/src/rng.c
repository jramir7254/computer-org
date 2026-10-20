#include "../include/rng.h"
#include "../include/game.h"

int lfsr()
{
    int bit3 = (current_seed >> 3) & 1;
    int bit2 = (current_seed >> 2) & 1;
    int xor = bit3 ^ bit2;
    current_seed = (current_seed << 1) | xor;
    return current_seed & 3;
}

void seed_generator()
{
    static int seed_tick = 1;

    game_seed = seed_tick;
    current_seed = game_seed;

    if (++seed_tick > 15)
    {
        seed_tick = 1;
    }
}