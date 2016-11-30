/************************************************************
Copyright (C) 1995-2007 Pumpkin, Inc. and its
Licensor(s).

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\Salvo\\Example\\MSP430\\MSP430x1xx\\TI\040MSP-TS430PM64\\Tut\\Tut5\\tut5.c,v $
$Author: aek $
$Revision: 3.2 $
$Date: 2008-02-06 20:52:23-08 $

Tutorial program Tut5.

See Salvo User Manual for in-depth description.

************************************************************/

#include "tut5.h"
#include <salvo.h>

#define TASK_COUNT_P             OSTCBP(1) 	// task #1
#define TASK_SHOW_P              OSTCBP(2) 	//  ""  #2
#define TASK_BLINK_P             OSTCBP(3) 	//  ""  #3

#define PRIO_COUNT               10        	// task priorities
#define PRIO_SHOW                10        	//  ""
#define PRIO_BLINK               2         	//  ""

#define BINSEM_UPDATE_PORT_P     OSECBP(1) 	// binSem #1

unsigned int counter;


void TaskCount (void)
{
    counter = 0;          // Initialized when tasks starts.


    while (1) {
        counter++;

        // Every time bit 8 of counter rolls over, signal the binSem.
        if (!(counter & 0x01FF)) {
            OSSignalBinSem(BINSEM_UPDATE_PORT_P);
        }

        // Context-switch so other tasks can run.
        OS_Yield();
    }
}


void TaskShow (void)
{
    while (1) {
        // Context-switch and wait for TaskCount() to signal that 512
        //  counts have elapsed.
        OS_WaitBinSem(BINSEM_UPDATE_PORT_P, OSNO_TIMEOUT);

        // Write upper 7 bits of 16-bit counter P1OUT:[7..1].
        P1OUT = (P1OUT & ~0xFE) | ((counter >> 8) & 0xFE);
    }
}


void TaskBlink (void)
{
    // Set P1OUT to all-outputs, initialize to 0x00. This only happens
    //  when the tasks starts.
    P1DIR = 0xFF;
    P1OUT = 0x00;

    while (1) {
        // Toggle P1OUT:BIT0
        P1OUT ^= BIT0;			

        // Context-switch and return in 50 ticks.
        OS_Delay(50); 							
    }
}


int main (void)
{
    // Stop WDT.
    WDTCTL = WDTPW + WDTHOLD;

    // TimerA init.
    TACTL  = TASSEL1 + TACLR;           // SMCLK, clear TAR.
    CCTL0  = CCIE;                      // CCR0 interrupt enabled.
    CCR0   = TIMERA0_RELOAD;            // 10ms ticks.
    TACTL |= MC1;                       // Start Timer_A in continuous mode.
	
    // Initialize Salvo.
    OSInit();

    // Create Salvo tasks.
    OSCreateTask(TaskCount, TASK_COUNT_P, PRIO_COUNT);
    OSCreateTask(TaskShow,  TASK_SHOW_P,  PRIO_SHOW );
    OSCreateTask(TaskBlink, TASK_BLINK_P, PRIO_BLINK);

    // Create Salvo events.
    OSCreateBinSem(BINSEM_UPDATE_PORT_P, 0);

    // Required: TimerA0 ISR calls OSTimer.
    __enable_interrupt();

    // Start multitasking.
    while (1) {
      OSSched();
    }
}
