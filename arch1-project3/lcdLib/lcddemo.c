/*
 * raycaster.c - a simplified, faster version of the raycasting test.
 *
 * Same idea as before (camera-plane raycaster + DDA grid stepping), but:
 *   - NO float and NO math.h: everything is integer "fixed-point" (x256)
 *   - turning uses a small sine table instead of sinf()/cosf()
 *   - no clearScreen(): each column draws ceiling + wall + floor, so every
 *     pixel is written once and there's no black flash
 *   - only columns that changed are redrawn
 *   - 64 rays drawn 2 pixels wide (half the work, still looks fine)
 *   - the frame is only redrawn when the player actually moves
 *   - buttons are polled from main (hold to keep moving), not handled in an ISR
 *
 * Controls (expansion board):  SW1 turn left   SW2 forward
 *                              SW3 backward    SW4 turn right
 *
 * Uses the course libraries: libTimer (clocks, watchdog tick) and lcdLib.
 */

#include <msp430.h>
#include <stdint.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

/* ================================================================ */
/*  Settings                                                        */
/* ================================================================ */

#define FIX 256                            /* fixed-point: 256 means 1.0 */
#define COL_WIDTH 2                        /* each ray draws a 2-pixel-wide strip */
#define NUM_COLS (screenWidth / COL_WIDTH) /* 64 rays */

#define MOVE_STEP 40  /* 40/256 = ~0.16 cells per update */
#define TURN_STEP 6   /* 6/256 of a full turn = ~8 degrees */
#define PLANE_LEN 169 /* 0.66 * 256: field of view (~66 degrees) */
#define MAX_DDA 64    /* safety limit on grid steps per ray */
#define INPUT_TICKS 8 /* check buttons every 8 WDT ticks (~30/sec) */

#define CEILING_COLOR COLOR_BLACK
#define FLOOR_COLOR COLOR_CHOCOLATE

#define RED_LED BIT6 /* on while the CPU is awake (P1.0 belongs to the LCD) */

#define SW_LEFT BIT0  /* P2.0 */
#define SW_FWD BIT1   /* P2.1 */
#define SW_BACK BIT2  /* P2.2 */
#define SW_RIGHT BIT3 /* P2.3 */
#define SWITCHES (SW_LEFT | SW_FWD | SW_BACK | SW_RIGHT)

/* ================================================================ */
/*  Map (stored in flash because it's const)                        */
/* ================================================================ */

#define MAP_W 24
#define MAP_H 24

static const uint8_t worldMap[MAP_W][MAP_H] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 0, 0, 0, 0, 5, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

/* Wall type -> color (index 0 unused: 0 means empty space) */
static const u_int wall_colors[] = {
    COLOR_YELLOW, COLOR_RED, COLOR_GREEN, COLOR_BLUE, COLOR_WHITE, COLOR_YELLOW};
#define NUM_WALL_COLORS (sizeof(wall_colors) / sizeof(wall_colors[0]))

/* ================================================================ */
/*  Fixed-point trig: angles are 0..255 for a full circle            */
/* ================================================================ */

/* sin of the first quarter circle, x256. The other 3 quarters are mirrors. */
static const int16_t quarter_sin[65] = {
    0,
    6,
    13,
    19,
    25,
    31,
    38,
    44,
    50,
    56,
    62,
    68,
    74,
    80,
    86,
    92,
    98,
    104,
    109,
    115,
    121,
    126,
    132,
    137,
    142,
    147,
    152,
    157,
    162,
    167,
    172,
    177,
    181,
    185,
    190,
    194,
    198,
    202,
    206,
    209,
    213,
    216,
    220,
    223,
    226,
    229,
    231,
    234,
    237,
    239,
    241,
    243,
    245,
    247,
    248,
    250,
    251,
    252,
    253,
    254,
    255,
    255,
    256,
    256,
    256,
};

static int16_t sin8(uint8_t a)
{
    uint8_t q = a & 63; /* position within the quarter */
    if (a & 64)
        q = 64 - q; /* 2nd and 4th quarters run backwards */
    int16_t v = quarter_sin[q];
    return (a & 128) ? -v : v; /* bottom half of the circle is negative */
}

static int16_t cos8(uint8_t a) { return sin8(a + 64); } /* cos = sin shifted 90 degrees */

/* ================================================================ */
/*  Player (all x256 fixed-point)                                   */
/* ================================================================ */

static int16_t posX = 13 * FIX + FIX / 2; /* middle of cell (13, 9) */
static int16_t posY = 9 * FIX + FIX / 2;
static uint8_t angle = 128; /* facing -X, like the original dir = (-1, 0) */

static int16_t dirX, dirY, planeX, planeY;

static void update_vectors(void)
{
    dirX = cos8(angle);
    dirY = sin8(angle);
    planeX = (int16_t)(((int32_t)dirY * PLANE_LEN) >> 8);  /* camera plane: 90 degrees */
    planeY = (int16_t)((-(int32_t)dirX * PLANE_LEN) >> 8); /* from dir, 0.66 long      */
}

static uint8_t is_wall(int16_t x, int16_t y)
{
    return worldMap[x >> 8][y >> 8] != 0; /* >> 8 turns x256 back into a cell number */
}

/* Read held buttons, move the player. Returns 1 if anything changed. */
static uint8_t update_player(void)
{
    uint8_t held = ~P2IN & SWITCHES; /* pressed = 0 with pull-ups, so invert */
    uint8_t moved = 0;

    if (held & SW_LEFT)
    {
        angle += TURN_STEP;
        moved = 1;
    } /* uint8_t wraps around by itself */
    if (held & SW_RIGHT)
    {
        angle -= TURN_STEP;
        moved = 1;
    }
    if (moved)
        update_vectors();

    int16_t step = 0;
    if (held & SW_FWD)
        step = MOVE_STEP;
    if (held & SW_BACK)
        step = -MOVE_STEP;

    if (step)
    {
        int16_t nx = posX + (int16_t)(((int32_t)dirX * step) >> 8);
        int16_t ny = posY + (int16_t)(((int32_t)dirY * step) >> 8);
        if (!is_wall(nx, posY))
        {
            posX = nx;
            moved = 1;
        } /* each axis separately: */
        if (!is_wall(posX, ny))
        {
            posY = ny;
            moved = 1;
        } /* lets you slide along walls */
    }
    return moved;
}

/* ================================================================ */
/*  Rendering                                                       */
/* ================================================================ */

/* What each column showed last frame, so unchanged columns can be skipped */
static uint8_t last_top[NUM_COLS];
static uint8_t last_look[NUM_COLS];

static void strip(u_char x, u_char y, u_char h, u_int color)
{
    if (h) /* fillRectangle(col, row, WIDTH, HEIGHT, color) */
        fillRectangle(x, y, COL_WIDTH, h, color);
}

static void render(void)
{
    for (uint8_t col = 0; col < NUM_COLS; col++)
    {

        /* camera x from -256 (left edge) to +256 (right edge), middle of the column */
        int16_t camX = (int16_t)((((int32_t)col * 2 + 1) * FIX) / NUM_COLS) - FIX;

        int16_t rayX = dirX + (int16_t)(((int32_t)planeX * camX) >> 8);
        int16_t rayY = dirY + (int16_t)(((int32_t)planeY * camX) >> 8);

        int16_t mapX = posX >> 8, mapY = posY >> 8;     /* which cell we're in */
        int16_t fracX = posX & 255, fracY = posY & 255; /* where inside that cell */

        /* distance to cross one whole cell in X / Y (x256). 65536 / ray = 256 * (256 / ray) */
        int32_t deltaX = rayX ? 65536L / (rayX < 0 ? -rayX : rayX) : 0x7FFFFFL;
        int32_t deltaY = rayY ? 65536L / (rayY < 0 ? -rayY : rayY) : 0x7FFFFFL;

        int8_t stepX, stepY;
        int32_t sideX, sideY; /* distance to the next X / Y line */
        if (rayX < 0)
        {
            stepX = -1;
            sideX = (fracX * deltaX) >> 8;
        }
        else
        {
            stepX = 1;
            sideX = ((FIX - fracX) * deltaX) >> 8;
        }
        if (rayY < 0)
        {
            stepY = -1;
            sideY = (fracY * deltaY) >> 8;
        }
        else
        {
            stepY = 1;
            sideY = ((FIX - fracY) * deltaY) >> 8;
        }

        /* DDA: hop to whichever grid line is closer until we're inside a wall */
        uint8_t side = 0, hit = 0;
        for (uint8_t i = 0; i < MAX_DDA && !hit; i++)
        {
            if (sideX < sideY)
            {
                sideX += deltaX;
                mapX += stepX;
                side = 0;
            }
            else
            {
                sideY += deltaY;
                mapY += stepY;
                side = 1;
            }
            hit = worldMap[mapX][mapY];
        }
        if (hit >= NUM_WALL_COLORS)
            hit = 0;

        /* straight-ahead distance (no fisheye), x256 */
        int32_t dist = side ? sideY - deltaY : sideX - deltaX;
        if (dist < 8)
            dist = 8; /* avoid giant / divide-by-zero */

        int32_t h = ((int32_t)screenHeight * FIX) / dist; /* wall height in pixels */
        if (h > screenHeight)
            h = screenHeight;
        uint8_t wall = (uint8_t)h;
        uint8_t top = (screenHeight - wall) / 2;

        /* skip the column if it looks exactly like last frame */
        uint8_t look = (uint8_t)(hit * 2 + side);
        if (top == last_top[col] && look == last_look[col])
            continue;
        last_top[col] = top;
        last_look[col] = look;

        u_int color = wall_colors[hit];
        if (side)
            color = (color >> 1) & 0x7BEF; /* darker: halve each of the B, G, R fields separately */

        u_char x = col * COL_WIDTH;
        strip(x, 0, top, CEILING_COLOR);
        strip(x, top, wall, color);
        strip(x, top + wall, screenHeight - top - wall, FLOOR_COLOR);
    }
}

/* ================================================================ */
/*  Main + tick                                                     */
/* ================================================================ */

int main(void)
{
    configureClocks(); /* stop watchdog, 16 MHz CPU, 2 MHz SMCLK */
    lcd_init();

    P1OUT &= ~RED_LED; /* "CPU busy" LED */
    P1DIR |= RED_LED;

    P2DIR &= ~SWITCHES; /* buttons: inputs with pull-ups, polled */
    P2REN |= SWITCHES;
    P2OUT |= SWITCHES;

    for (uint8_t c = 0; c < NUM_COLS; c++)
        last_top[c] = 0xFF; /* force every column to draw the first time */

    update_vectors();
    render();

    enableWDTInterrupts(); /* ~250 ticks per second */

    while (1)
    {
        P1OUT &= ~RED_LED;
        __bis_SR_register(LPM0_bits | GIE); /* sleep until the tick wakes us */
        P1OUT |= RED_LED;

        if (update_player()) /* only draw when something changed */
            render();
    }
}

void __interrupt_vec(WDT_VECTOR) WDT(void)
{
    static uint8_t ticks = 0;
    if (++ticks >= INPUT_TICKS)
    {
        ticks = 0;
        __bic_SR_register_on_exit(CPUOFF); /* wake main to check the buttons */
    }
}