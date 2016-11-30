/******************************************************************************
(C) Copyright Pumpkin, Inc. All Rights Reserved.

This file may be distributed under the terms of the License 
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, 
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND 
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\Library\\MSP430\\Src\\power.c,v $
$Author: aek $
$Revision: 3.3 $
$Date: 2009-11-01 22:53:49-08 $

******************************************************************************/
#include "config.h"
#include "power.h"


/******************************************************************************
****                                                                       ****
**                                                                           **
shutdown()

**                                                                           **
****                                                                       ****
******************************************************************************/
void shutdown(void) {

  // Reset MCLK select, turn XT2 off.
  #ifdef __MSP430_HAS_BASIC_CLOCK__
  BCSCTL2 =    0;
  BCSCTL1 = 0x84; 
  #endif

  // Stop watchdog (conserves power).
  WDTCTL = WDTPW + WDTHOLD;   

  // ADC12 off.  
  ADC12CTL0 &= ~ENC;
  ADC12CTL0  =    0;       
  ADC12CTL1  =    0;                
  ADC12MCTL0 =    0;  

  // Turn USARTs off.
  #ifdef __MSP430_HAS_UART0__
  UCTL0   =   SWRST; 
  ME1     =       0;
  #endif
  #ifdef __MSP430_HAS_UART1__
  UCTL1   =   SWRST; 
  ME2     =       0;
  #endif

  // disable all interrupt sources.
  IE1 = 0;
  IE2 = 0;
  __disable_interrupt();

  // Go to sleep forever.
  while(1) {
    LPM4;
  }
  
} /* shutdown() */
