/******************************************************************************
(C) Copyright Pumpkin, Inc. All Rights Reserved.

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\Library\\MSP430\\Src\\error.c,v $
$Author: aek $
$Revision: 3.4 $
$Date: 2009-11-01 22:53:54-08 $

******************************************************************************/
#include "config.h"
#include "IO.h"
#include <in430.h>
#include <msp430.h>


/* derived empirically. */
#define ERROR_FLASH_HIGH     4000
#define ERROR_FLASH_LOW     26000
#define ERROR_FLASH_QUIET   50000

/******************************************************************************
****                                                                       ****
**                                                                           **
fatal_delay()

Delay function for fatal error handler.

Algorithm:  simple inline delay, but tweaked for easy
             viewing of the blink count by padding with
             NOPs.

Notes:      Assumes MCLK = DCO.
            Padding with NOPs allows us to avoid long int
             (i.e. 32-bit) math, for a code size savings of
             of two instructions overall (including calls).

Returns:    --

**                                                                           **
****                                                                       ****
******************************************************************************/
static
void fatal_delay(unsigned int count) {

  do {
    _NOP();
    _NOP();
    _NOP();
    _NOP();
  } while (--count);

} /* fatal_delay() */


/******************************************************************************
****                                                                       ****
**                                                                           **
fatal()

Fatal error handler.

Algorithm:  disable global interrupts
            set MCLK to DCO
            if <count>==0
              set LED high forever
            else
              blink LED <count> times
              delay
              repeat

Notes:      This is terminal. Code will never return.

Returns:    --

**                                                                           **
****                                                                       ****
******************************************************************************/
void fatal(unsigned int count) {

  unsigned int j;


  __disable_interrupt();

  #ifdef __MSP430_HAS_BASIC_CLOCK__
  BCSCTL2 &= ~(SELM1+SELM0);
  #endif

  INIT_HEARTBEAT_LED();

  if (count == 0) {
    TURN_HEARTBEAT_LED_ON();

    do { ; } while (TRUE);
  }

  else {
    do {
      j = count;

      do {
        TURN_HEARTBEAT_LED_ON();
        fatal_delay(ERROR_FLASH_HIGH);
        TURN_HEARTBEAT_LED_OFF();
        fatal_delay(ERROR_FLASH_LOW);
      } while (--j);

      fatal_delay(ERROR_FLASH_QUIET);

    } while (TRUE);
  } /* else */

} /* fatal() */
