/* simpledemo.c — a stripped-down version of wakedemo
 *
 * A white square slides left to right across a blue screen.
 * Hold SW1 (P2.0) on the LCD board to turn it yellow.
 *
 * Same idea as wakedemo:
 *   - the watchdog interrupt does the quick math and says "redraw"
 *   - main does the slow drawing, then goes back to sleep
 */

#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

#define SW1  BIT0          /* first button on the LCD board: P2.0 */
#define SIZE 10            /* square is 10 x 10 pixels */
#define ROW  70            /* square stays on this row */

/* Shared between the interrupt and main, so they must be volatile */
volatile int squareCol   = 0;            /* where the square SHOULD be */
volatile u_int squareColor = COLOR_WHITE;
volatile int needRedraw  = 1;            /* 1 = main has drawing to do */

int main(void)
{
  /* ---- Step 1: setup ---- */
  configureClocks();               /* 16 MHz CPU, stop watchdog */
  lcd_init();                      /* start the screen */
  clearScreen(COLOR_BLUE);

  P2REN |= SW1;                    /* button: pull-up resistor on */
  P2OUT |= SW1;                    /*   (reads 1 when not pressed) */
  P2DIR &= ~SW1;                   /*   input */

  enableWDTInterrupts();           /* ~250 interrupts per second */
  or_sr(0x8);                      /* GIE: allow interrupts */

  int drawnCol = squareCol;        /* where the square IS on screen */

  /* ---- Step 2: draw when told to, otherwise sleep ---- */
  while (1) {
    if (needRedraw) {
      needRedraw = 0;
      fillRectangle(drawnCol, ROW, SIZE, SIZE, COLOR_BLUE);    /* erase old */
      drawnCol = squareCol;
      fillRectangle(drawnCol, ROW, SIZE, SIZE, squareColor);   /* draw new */
    }
    or_sr(0x10);                   /* CPU off until the interrupt wakes us */
  }
}

/* ---- Step 3: the watchdog interrupt (about 250 times per second) ---- */
void __interrupt_vec(WDT_VECTOR) WDT()
{
  static int ticks = 0;
  if (++ticks < 25)                /* only act every 25 ticks (~10 per second) */
    return;                        /* returning here: CPU goes back to sleep */
  ticks = 0;

  /* move the square right; wrap to the left edge at the end */
  squareCol += 2;
  if (squareCol > screenWidth - SIZE)
    squareCol = 0;

  /* button pressed reads 0 */
  if (P2IN & SW1)
    squareColor = COLOR_WHITE;
  else
    squareColor = COLOR_YELLOW;

  needRedraw = 1;
  __bic_SR_register_on_exit(CPUOFF);   /* wake main up when we return */
}

