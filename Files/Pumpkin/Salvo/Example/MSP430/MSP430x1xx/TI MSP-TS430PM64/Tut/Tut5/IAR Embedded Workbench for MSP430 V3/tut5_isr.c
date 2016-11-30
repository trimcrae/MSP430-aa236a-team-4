/************************************************************
Copyright (C) 1995-2007 Pumpkin, Inc. and its
Licensor(s).

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\Salvo\\Example\\MSP430\\MSP430x1xx\\TI\040MSP-TS430PM64\\Tut\\Tut5\\IAR\040Embedded\040Workbench\040for\040MSP430\040V3\\tut5_isr.c,v $
$Author: aek $
$Revision: 3.0 $
$Date: 2008-02-06 20:52:53-08 $

************************************************************/

#include "tut5.h"
#include <salvo.h>

#define LOOP_HERE()             do { ; } while (1)


/************************************************************
****                                                     ****
**                                                         **
Active ISRs.

TimerA0: Interrupt happens at predefined system tick rate,
          calls OSTimer(). Since this ISR calls a Salvo
          service, it (and all other ISRs that call Salvo
          services) must be disabled by Salvo's interrupt
          hooks.

**                                                         **
****                                                     ****
************************************************************/
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
  CCR0 += TIMERA0_RELOAD;
  OSTimer();
}


/************************************************************
****                                                     ****
Other (unused) ISRs .

Unused ISRs are not listed in order to achieve maximum
compatibility across all MSP430 families.

**                                                         **
****                                                     ****
************************************************************/

