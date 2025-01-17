// This code was ported from TI's sample code. See Copyright notice at the bottom of this file.

#include "msp430fr6989.h"
#include "LcdDriver/middle_driver.h"
#include "Grlib/grlib/grlib.h"
#include <stdint.h>

void HAL_LCD_PortInit(void)
{
    /////////////////////////////////////
    // Configuring the SPI pins
    /////////////////////////////////////

    // Divert UCB0CLK/P1.4 pin to serial clock
    P1SEL0 |= BIT4; //SC is at P1.4

    // Divert UCB0SIMO/P1.6 pin to SIMO

    P1SEL0 |= BIT6; // Serial Data out is located at P1.6

    // OK to ignore UCB0STE/P1.5 since we'll connect the display's enable bit to low (enabled all the time)
    // OK to ignore UCB0SOMI/P1.7 since the display doesn't give back any data

    ///////////////////////////////////////////////
    // Configuring the display's other pins
    ///////////////////////////////////////////////

    // Set reset pin as output
    P9DIR |= BIT4; // Reset pin is located at 9.4

    // Set the data/command pin as output (DC at 2.5)
    P2DIR |= BIT3;

    // Set the chip select pin as output (Located at Port 2.5)
    P2DIR |= BIT5;

    return;
}

void HAL_LCD_SpiInit(void)
{
    //////////////////////////
    // SPI configuration
    //////////////////////////

    //eUSCI Set to reset state
    UCB0CTLW0 |= UCSWRST;

    //Clock phase set to capture on 1st edge, change on following edge
    UCB0CTLW0 |= UCCKPH;

    // Clock polarity set to idle at low
    UCB0CTLW0 &= ~UCCKPL;

    // Transmit MSB first
    UCB0CTLW0 |= UCMSB;

    // MCU set to master
    UCB0CTLW0 |= UCMST;

    //SPI set to 3 pin SPI
    UCB0CTLW0 |= UCMODE_0;

    // Set module to synchronous mode
    UCB0CTLW0 |= UCSYNC;

    // Clock set to SMCLK
    UCB0CTLW0 |= UCSSEL_2;

    // Set clock divider to 1 (SMCLK is from DCO stays at 8 MHz)
    UCB0BRW = 0;

    // Exit the reset state
    UCB0CTLW0 &= ~UCSWRST;

    // Set CS (chip select) bit to 0 (display always enabled at P2.5)
    P2OUT &= ~BIT5;

    // Set DC bit to 0 (assume data) at P2.3
    P2OUT &= ~BIT3;
    return;
}


//*****************************************************************************
// Writes a command to the CFAF128128B-0145T.  This function implements the basic SPI
// interface to the LCD display.
//*****************************************************************************
void HAL_LCD_writeCommand(uint8_t command)
{
    // For command, set the DC' bit to low before transmission
    P2OUT &= ~BIT3;

    // Wait as long as the module is busy
    while (UCB0STATW & UCBUSY);

    // Transmit data
    UCB0TXBUF = command;

    // Set DC' bit back to high
    P2OUT |= BIT3;
}


//*****************************************************************************
// Writes a data to the CFAF128128B-0145T.  This function implements the basic SPI
// interface to the LCD display.
//*****************************************************************************
void HAL_LCD_writeData(uint8_t data)
{
    // Wait as long as the module is busy
    while (UCB0STATW & UCBUSY);

    // Transmit data
    UCB0TXBUF = data;
}

/* --COPYRIGHT--,BSD
 * Copyright (c) 2015, Texas Instruments Incorporated
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
