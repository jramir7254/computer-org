/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
//******************************************************************************
//  MSP430FR2000 RGB LED Demo
//
//  Description: Basic RGB LED control using PWMs.
//  An RGB LED is controlled by TB0.1 (Red), TB0.2 (Green) and TB0.0 (Blue)
//  PWM outputs. This code uses color-mixing to produce one of 12 different
//  colors, selected by sending a number 0h-Bh over UART to the MSP430.
//
//  Because the Timer B on MSP430FR2000 only has 3 CCR registers, and TB0CCR0
//  cannot produce PWM with variable duty cycle in up-down mode (CCR0 sets
//  period) continuous mode is used instead. The theory behind generating PWMs
//  in this way is explained in detail in the application note SLAA513,
//  "Multiple Time Bases on a Single MSP430 Timer Module".
//
//  XT1 is considered to be absent in this example.
//  ACLK = default REFO ~32768Hz, MCLK = SMCLK = default DCODIV ~1.048MHz.
//
//           MSP430FR2000
//         ---------------
//     /|\|               |
//      | |               |
//      --|RST            |
//        |               |
//        |           P1.7|--> UCA0TXD
//        |           P1.6|<-- UCA0RXD
//        |               |
//        |           P2.0|--> Red (TB0.1)
//        |           P2.1|--> Green (TB0.2)
//        |           P1.0|--> Blue (TB0.0)
//
//   Katie Pier
//   Texas Instruments Inc.
//   August 2017
//   Built with IAR Embedded Workbench v7.1 & Code Composer Studio v7.2
//******************************************************************************
#include <msp430.h>
#include <stdint.h>

/*
 * colorsLow and colorsHigh are a lookup table for the duration of timer high
 * and low periods for the Red, Green, and Blue LEDs to generate each color
 * selection. The addition of a colorsLow entry with the corresponding
 * colorsHigh entry must be 135, as this is the total period to generate the
 * desired 60Hz LED frequency.
 *
 * Varying the ratio of colorsLow to colorsHigh while keeping the sum of 135
 * changes the PWM duty cycle for the R, G, or B LED and thereby changes the
 * intensity. The color is modified by changing the intensity of the R, G, and B
 * LEDs in relation to each other to produce the desired color mixing. It is
 * also important to avoid a complete 100% or 0% duty cycle in order for the
 * timer ISR handling to operate correctly, which is why there are entries of 1
 * and 134 instead of 0 and 135.
 *
 * Experimentation may be needed with a particular RGB LED in order to get the
 * correct color mixing for that LED, as intensities of different colors can
 * vary. For the LED used in the example, the Red LED was dimmer than the others
 * and therefore other color values had to be dimmed in comparison in order to
 * achieve the desired color (see the orange and purple entries in the table)
 */

//  Contains the low (on) period for the RGB LEDs for each color selection
const uint8_t colorsLow[12][3] = {
    //{R, G, B}
    {134, 1, 1},   // RED
    {134, 15, 1},  // RED-ORANGE
    {134, 20, 1},  // ORANGE
    {134, 30, 1},  // ORANGE-YELLOW
    {134, 55, 1},  // YELLOW
    {67, 80, 1},   // YELLOW-GREEN
    {1, 134, 1},   // GREEN
    {1, 134, 134}, // GREEN-BLUE
    {1, 1, 134},   // BLUE
    {67, 1, 134},  // BLUE-PURPLE
    {134, 1, 134}, // PURPLE
    {134, 1, 80}   // PURPLE-RED
};
//  Contains the high (off) period for the RGB LEDs for each color selection
const uint8_t colorsHigh[12][3] = {
    //{R, G, B}
    {1, 134, 134}, // RED
    {1, 120, 134}, // RED-ORANGE
    {1, 115, 134}, // ORANGE
    {1, 100, 134}, // ORANGE-YELLOW
    {1, 80, 134},  // YELLOW
    {68, 55, 134}, // YELLOW-GREEN
    {134, 1, 134}, // GREEN
    {134, 1, 1},   // GREEN-BLUE
    {134, 134, 1}, // BLUE
    {68, 134, 1},  // BLUE-PURPLE
    {1, 134, 1},   // PURPLE
    {1, 134, 55}   // PURPLE-RED
};

__no_init uint8_t selected; // Used to track the current color selection

/**
 * main.c
 */
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer

    selected = 0; // Default to Red

    // Configure UART
    // From user's Guide Table of Baud Rates, 9600 baud at BRCLK = 32768
    // UCOS16 = 0
    // UCBRx = 3
    // UCBRFx = 0
    // UCBRSx = 0x92
    UCA0CTLW0 = UCSWRST | UCSSEL__ACLK;
    UCA0BRW = 3;
    UCA0MCTLW = 0x9200;
    UCA0CTLW0 &= ~UCSWRST; // Initialize eUSCI
    UCA0IE = UCRXIE;       // Enable USCI_A0 RX interrupt

    // Configure GPIO
    PASEL0 = BIT6 | BIT7 | BIT8 | BIT9; // TB0.1 and 0.2, UCA0 RXD and TXD
    PADIR = BIT8 | BIT9 | BIT0;         // TB0.1, 0.2, 0.0 (on P1.0)
    PAOUT = 0;

    SYSCFG3 = TBRMP; // Re-Map timer B to P2.0/1

    PM5CTL0 &= ~LOCKLPM5; // Disable the GPIO power-on default
                          // high-impedance mode to activate
                          // previously configured port settings

    // Set up Timer B in continuous mode for multiple time base PWMs.
    // See app note SLAA513 for theory.
    TB0CCTL0 = CCIS_0 + OUTMOD_4 + CCIE; // CCR0 toggle, interrupt enabled
    TB0CCTL1 = CCIS_0 + OUTMOD_4 + CCIE; // CCR1 toggle, interrupt enabled
    TB0CCTL2 = CCIS_0 + OUTMOD_4 + CCIE; // CCR2 toggle, interrupt enabled

    // REF0CLK/4 = 8192 is used for TBCLK so that the period of 60Hz can be
    // generated with TB0CCRx values totaling only 135 for the period of 60Hz
    // PWM. This is less than 255, so that the lookup table can be made of 8-bit
    // values, saving additional FRAM space on small devices.
    TB0CTL = TBSSEL__ACLK + ID__4 + MC_2 + TBCLR; // ACLK, contmode, clear TAR

    __bis_SR_register(LPM3_bits | GIE); // Go to LPM3 with interrupts
    __no_operation();
}

// UART interrupt service routine
#pragma vector = USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
    selected = UCA0RXBUF; // Get new value for selected color

    if (selected > 11) // If value out of range, set default 0 (red)
    {
        selected = 0;
    }
}

// Timer_B0 interrupt service routine
#pragma vector = TIMER0_B0_VECTOR
__interrupt void TIMER0_B0_ISR(void)
{
    if (P1OUT & BIT0) // Check current output level
    {
        TB0CCR0 += colorsLow[selected][2]; // Low period (Blue LED on)
        P1OUT = 0;                         // TB0.0 not brought out on pin, so set manually
    }
    else
    {
        TB0CCR0 += colorsHigh[selected][2]; // High period (Blue LED off)
        P1OUT = BIT0;                       // TB0.0 not brought out on pin, so set manually
    }
}

// Timer_B1 Interrupt Vector (TBIV) handler
#pragma vector = TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void)
{
    switch (__even_in_range(TB0IV, 14))
    {
    case 0:
        break;
    case 2:
        if (TB0CCTL1 & CCI) // Check current timer pin state
        {
            TB0CCR1 += colorsHigh[selected][0]; // High period (Red off)
        }
        else
        {
            TB0CCR1 += colorsLow[selected][0]; // Low period (Red on)
        }
        break;
    case 4:
        if (TB0CCTL2 & CCI) // Check current timer pin state
        {
            TB0CCR2 += colorsHigh[selected][1]; // High (Green off)
        }
        else
        {
            TB0CCR2 += colorsLow[selected][1]; // Low period (Green on)
        }
        break;
    default:
        break;
    }
}

// // Alternate LEDs from Off, Green, and Red
// #include <msp430.h>
// #include "libTimer.h"
// #include "led.h"

// int main(void)
// {
//     P2DIR |= (BIT1 | BIT5 | BIT3);
//     // P2OUT &= ~(BIT1 | BIT5 | BIT3);
//     P2OUT |= (BIT1 | BIT5);

//     or_sr(0x18); /* CPU off, GIE on */
// }
