/*
 * Joystick-controlled LEDs for the MSP-EXP430G2ET (MSP430G2553), no LCD board.
 * Everything is in this one file; it does not need libTimer.
 *
 * WIRING (joystick module -> LaunchPad)
 *   GND  -> GND
 *   +5V  -> 3V3      (NOT 5V: the pins can only take ~3.6 V)
 *   VRx  -> P1.4     (analog channel A4)
 *   VRy  -> P1.5     (analog channel A5)
 *   SW   -> P1.7     (digital input, internal pull-up)
 *   Keep jumpers J6 (green LED, P1.0) and J7 (red LED, P1.6) on.
 *   Leave the stick untouched while powering up: the center is calibrated then.
 *
 * WHAT IT DOES
 *   Push left   -> green LED, brighter the farther you push
 *   Push right  -> red LED,   brighter the farther you push
 *   Push up/down-> both LEDs, brighter the farther you push
 *   Click stick -> toggle blink mode (LEDs flash on/off twice a second)
 *
 * HOW IT WORKS
 *   Timer_A interrupts 16,000 times a second and does software PWM on both
 *   LEDs (64 brightness steps, ~250 Hz, no flicker). Every 320 interrupts
 *   (~50 times a second) it wakes main(), which reads the joystick with the
 *   ADC, works out the new brightness, and goes back to sleep.
 *
 * BUILD / LOAD
 *   msp430-elf-gcc -mmcu=msp430g2553 -Os -I/opt/ti/msp430-gcc/include -L/opt/ti/msp430-gcc/include -o main.elf main.c
 *   mspdebug tilib "prog main.elf"
 */

#include <msp430.h>
#include <stdint.h>

/* ---------------- pins ---------------- */
#define LED_GREEN BIT0 /* P1.0 */
#define LED_RED BIT6   /* P1.6 */
#define LEDS (LED_GREEN | LED_RED)

#define JOY_X INCH_4 /* VRx on P1.4 = A4 */
#define JOY_Y INCH_5 /* VRy on P1.5 = A5 */
#define JOY_ANALOG (BIT4 | BIT5)
#define JOY_SW BIT7 /* SW on P1.7 */

/* ---------------- timing ---------------- */
#define SMCLK_HZ 2000000UL                         /* 16 MHz DCO / 8 */
#define PWM_TICK_HZ 16000UL                        /* Timer_A interrupt rate */
#define PWM_STEPS 64                               /* brightness levels: 0..64 (power of 2) */
#define CONTROL_HZ 50                              /* how often main() reads the joystick */
#define CONTROL_DIVIDER (PWM_TICK_HZ / CONTROL_HZ) /* 320 timer ticks */
#define BLINK_UPDATES 25                           /* 25 updates at 50 Hz = 0.5 s */

/* ---------------- joystick ---------------- */
#define DEADZONE 60 /* ignore wobble near the center */
#define ADC_MAX 1023

/* Written by main(), read by the timer interrupt: must be volatile.
   uint8_t writes are a single instruction, so no tearing. */
static volatile uint8_t red_duty = 0; /* 0 = off .. PWM_STEPS = full */
static volatile uint8_t green_duty = 0;

static uint16_t center_x = ADC_MAX / 2;
static uint16_t center_y = ADC_MAX / 2;

/* ================================================================ */
/*  Setup                                                           */
/* ================================================================ */

static void clocks_init(void)
{
    WDTCTL = WDTPW | WDTHOLD; /* stop the watchdog FIRST */
    BCSCTL1 = CALBC1_16MHZ;   /* factory-calibrated 16 MHz DCO */
    DCOCTL = CALDCO_16MHZ;
    BCSCTL2 = DIVS_3; /* SMCLK = DCO / 8 = 2 MHz */
}

static void leds_init(void)
{
    P1OUT &= ~LEDS; /* value first... */
    P1DIR |= LEDS;  /* ...then outputs */
}

static void joystick_init(void)
{
    /* analog axes */
    ADC10CTL0 = SREF_0       /* measure 0 V .. VCC */
                | ADC10SHT_2 /* sample for 16 ADC clocks */
                | ADC10ON;
    ADC10CTL1 = ADC10SSEL_0; /* ADC's own clock, single conversion */
    ADC10AE0 |= JOY_ANALOG;  /* P1.4, P1.5 = analog inputs */

    /* push button: input with pull-up, pressed reads 0 */
    P1DIR &= ~JOY_SW;
    P1REN |= JOY_SW;
    P1OUT |= JOY_SW;
}

static void pwm_timer_init(void)
{
    TA0CCR0 = (SMCLK_HZ / PWM_TICK_HZ) - 1; /* 2,000,000 / 16,000 = 125 counts */
    TA0CCTL0 = CCIE;                        /* interrupt when the count is reached */
    TA0CTL = TASSEL_2 | MC_1 | TACLR;       /* SMCLK, count up, start from 0 */
}

/* ================================================================ */
/*  Joystick reading                                                */
/* ================================================================ */

/* One conversion: 0 (0 V) .. 1023 (3.3 V). Takes a few microseconds. */
static uint16_t adc_read(uint16_t channel)
{
    ADC10CTL0 &= ~ENC; /* off to change channel */
    ADC10CTL1 = (ADC10CTL1 & ~INCH_15) | channel;
    ADC10CTL0 |= ENC | ADC10SC; /* start */
    while (ADC10CTL1 & ADC10BUSY)
        ;
    return ADC10MEM;
}

/* Signed distance from center, with the dead zone removed: about -512..+511 */
static int16_t axis(uint16_t channel, uint16_t center)
{
    int16_t d = (int16_t)adc_read(channel) - (int16_t)center;
    if (d > -DEADZONE && d < DEADZONE)
        return 0;
    return d;
}

/* Distance (0..~512) -> brightness (0..PWM_STEPS). 512 / 8 = 64. */
static uint8_t to_duty(int16_t distance)
{
    if (distance < 0)
        distance = -distance;
    distance >>= 3;
    return (distance > PWM_STEPS) ? PWM_STEPS : (uint8_t)distance;
}

static uint8_t max_u8(uint8_t a, uint8_t b) { return (a > b) ? a : b; }

/* Called ~50 times a second from main() */
static void update_from_joystick(void)
{
    static uint8_t blink_mode = 0;
    static uint8_t was_pressed = 0;
    static uint8_t blink_count = 0;
    static uint8_t blink_on = 1;

    /* button: act once per press (polling at 50 Hz also debounces it) */
    uint8_t pressed = !(P1IN & JOY_SW);
    if (pressed && !was_pressed)
    {
        blink_mode = !blink_mode;
        blink_on = 1;
        blink_count = 0;
    }
    was_pressed = pressed;

    /* stick position -> brightness */
    int16_t x = axis(JOY_X, center_x);
    int16_t y = axis(JOY_Y, center_y);

    uint8_t left = (x < 0) ? to_duty(x) : 0;
    uint8_t right = (x > 0) ? to_duty(x) : 0;
    uint8_t vert = to_duty(y);

    uint8_t red = max_u8(right, vert);
    uint8_t green = max_u8(left, vert);

    /* blink mode: hide the LEDs every other half second */
    if (blink_mode && ++blink_count >= BLINK_UPDATES)
    {
        blink_count = 0;
        blink_on = !blink_on;
    }
    if (blink_mode && !blink_on)
        red = green = 0;

    red_duty = red;
    green_duty = green;
}

/* ================================================================ */
/*  Interrupt: software PWM + "time to update" wake-up               */
/* ================================================================ */

void __interrupt_vec(TIMER0_A0_VECTOR) pwm_tick(void)
{
    static uint8_t phase = 0;
    static uint16_t control_count = 0;

    phase = (phase + 1) & (PWM_STEPS - 1); /* 0..63, wraps for free */

    if (phase < red_duty)
        P1OUT |= LED_RED;
    else
        P1OUT &= ~LED_RED;
    if (phase < green_duty)
        P1OUT |= LED_GREEN;
    else
        P1OUT &= ~LED_GREEN;

    if (++control_count >= CONTROL_DIVIDER)
    {
        control_count = 0;
        __bic_SR_register_on_exit(LPM0_bits); /* wake main() when we return */
    }
}

/* ================================================================ */
/*  Main                                                            */
/* ================================================================ */

int main(void)
{
    clocks_init();
    leds_init();
    joystick_init();

    center_x = adc_read(JOY_X); /* calibrate while the stick is at rest */
    center_y = adc_read(JOY_Y);

    pwm_timer_init(); /* start the timer last... */

    while (1)
    {
        __bis_SR_register(LPM0_bits | GIE); /* ...sleep (interrupts on) until woken */
        update_from_joystick();             /* slow work happens here, not in the ISR */
    }
}