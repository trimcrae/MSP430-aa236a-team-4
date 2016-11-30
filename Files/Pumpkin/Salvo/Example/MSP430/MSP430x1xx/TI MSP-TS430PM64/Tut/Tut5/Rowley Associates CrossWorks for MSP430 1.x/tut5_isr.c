/************************************************************
Copyright (C) 1995-2007 Pumpkin, Inc. and its
Licensor(s). 

This file may be distributed under the terms of the License 
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, 
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND 
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\Salvo\\Example\\MSP430\\MSP430x1xx\\TI\040MSP-TS430PM64\\Tut\\Tut5\\Rowley\040Associates\040CrossWorks\040for\040MSP430\0401.x\\tut5_isr.c,v $
$Author: aek $
$Revision: 3.0 $
$Date: 2008-02-06 20:52:36-08 $

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
void Timer_A (void) __interrupt[TIMERA0_VECTOR]
{
	CCR0 += TIMERA0_RELOAD; 
  OSTimer(); 
}                      


/************************************************************
****                                                     ****
**                                                         **
Unused ISRs.

NOTE: If your MSP430 does not have any of these vectors,
just comment it out to successfully build the project.

**                                                         **
****                                                     ****
************************************************************/
void ISRTx0 (void) __interrupt[UART0TX_VECTOR]
{
	LOOP_HERE();
} 


void ISRTx1 (void) __interrupt[UART1TX_VECTOR]
{
	LOOP_HERE();
} 


void ISRRx0 (void) __interrupt[UART0RX_VECTOR]
{
	LOOP_HERE();
}


void ISRRx1 (void) __interrupt[UART1RX_VECTOR]
{
	LOOP_HERE();
}


void ISRPort2 (void) __interrupt[PORT2_VECTOR]
{
	LOOP_HERE();	
}


void ISRPort1 (void) __interrupt[PORT1_VECTOR]
{
	LOOP_HERE();	
}


void ISRTimerA1 (void) __interrupt[TIMERA1_VECTOR]
{
	LOOP_HERE();
}


void ISRADC (void) __interrupt[ADC_VECTOR]
{
	LOOP_HERE();	
}


void ISRWDT (void) __interrupt[WDT_VECTOR]
{
	LOOP_HERE();	
}

void ISRCMPA (void) __interrupt[COMPARATORA_VECTOR]
{
	LOOP_HERE();	
}


void ISRTimerB1 (void) __interrupt[TIMERB1_VECTOR]
{
	LOOP_HERE();	
}


void ISRTimerB0 (void) __interrupt[TIMERB0_VECTOR]
{
	LOOP_HERE();	
}


void ISRNMI (void) __interrupt[NMI_VECTOR]
{
	LOOP_HERE();	
}

