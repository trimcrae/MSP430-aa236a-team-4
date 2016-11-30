/************************************************************
Copyright (C) 1995-2007 Pumpkin, Inc. and its
Licensor(s). 

This file may be distributed under the terms of the License 
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, 
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND 
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\Salvo\\Example\\MSP430\\MSP430x4xx\\TI\040MSP-TS430PZ100\\Tut\\Tut5\\IAR\040Embedded\040Workbench\040for\040MSP430\040V3\\tut5_isr.c,v $
$Author: aek $
$Revision: 3.0 $
$Date: 2008-02-06 20:50:54-08 $

************************************************************/

#include "tut5.h"
#include <salvo.h>


#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
    TACCR0 = TAR+TMR_TICK_10ms;  // reload timer A cc0 with the kernel timer tick
    OSTimer();
}


/************************************************************
****                                                     ****
Other (unused) ISRs -- trap for debugging purposes.

**                                                         **
****                                                     ****
************************************************************/
#pragma vector=USCIAB0TX_VECTOR
__interrupt void ISRUART0TX (void)
{
	LOOP_HERE();
}

#pragma vector=USART1TX_VECTOR
__interrupt void ISRUART1TX (void)
{
	LOOP_HERE();
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void ISRUART0RX (void)
{
	LOOP_HERE();
}

#pragma vector=USART1RX_VECTOR
__interrupt void ISRUART1RX (void)
{
	LOOP_HERE();
}

#pragma vector=PORT2_VECTOR
__interrupt void ISRPort2 (void)
{
	LOOP_HERE();
}


#pragma vector=PORT1_VECTOR
__interrupt void ISRPort1 (void)
{
	LOOP_HERE();
}


#pragma vector=TIMERA1_VECTOR
__interrupt void ISRTimerA1 (void)
{
	LOOP_HERE();
}


#pragma vector=ADC12_VECTOR
__interrupt void ISRADC (void)
{
	LOOP_HERE();
}


#pragma vector=WDT_VECTOR
__interrupt void ISRWDT (void)
{
	LOOP_HERE();
}


#pragma vector=COMPARATORA_VECTOR
__interrupt void ISRCMPA (void)
{
	LOOP_HERE();
}


#pragma vector=TIMERB1_VECTOR
__interrupt void ISRTimerB1 (void)
{
	LOOP_HERE();
}


#pragma vector=TIMERB0_VECTOR
__interrupt void ISRTimerB0 (void)
{
	LOOP_HERE();
}


#pragma vector=NMI_VECTOR
__interrupt void ISRNMI (void)
{
	LOOP_HERE();
}
