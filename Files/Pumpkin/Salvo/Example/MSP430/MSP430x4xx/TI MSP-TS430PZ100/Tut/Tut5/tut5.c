/************************************************************
Copyright (C) 1995-2007 Pumpkin, Inc. and its
Licensor(s).

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\Salvo\\Example\\MSP430\\MSP430x4xx\\TI\040MSP-TS430PZ100\\Tut\\Tut5\\tut5.c,v $
$Author: aek $
$Revision: 3.1 $
$Date: 2008-02-06 20:53:40-08 $

Tutorial program Tut5.

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
        if (!(counter & 0x01FF))
        {
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

        // Write upper 6 bits of 16-bit counter P5OUT:[7..2].
        P5OUT = (P5OUT & ~0xFD) | ((counter >> 8) & 0xFD);
    }
}


void TaskBlink (void)
{
    // Set P5OUT to all-outputs, initialize to 0x00. This only happens
    //  when the tasks starts.
    P5DIR = 0xFF;
    P5OUT = 0x00;

    while (1) {
        // Toggle P5OUT.BIT1 (LED on MSP-TS430PZ100 Target Socket Module).
        P5OUT ^= BIT1;			

        // Context-switch and return in 50 ticks.
        OS_Delay(50); 							
    }
}


int main (void)
{
    WDTCTL   =     WDTPW+WDTHOLD;       // Stop WDT.

    TACTL    =  TASSEL_2 + TACLR;       // Source from SMCLK, reset timer A.
    TACCR0   = TAR+TMR_TICK_10ms;       // Set timer A cc0 with the kernel timer tick.
    TACCTL0 &=            ~CCIFG;       // Clear timer A cc0 irq flag.
    TACCTL0 |=              CCIE;       // Enable timer A cc0 irq.
    TACTL   |=              MC_2;       // Start timer A in the continous mode.
	
    OSInit();                           // Initialize Salvo.

    OSCreateTask(TaskCount, TASK_COUNT_P, PRIO_COUNT);  // Create Salvo tasks.
    OSCreateTask(TaskShow,  TASK_SHOW_P,  PRIO_SHOW );
    OSCreateTask(TaskBlink, TASK_BLINK_P, PRIO_BLINK);

    OSCreateBinSem(BINSEM_UPDATE_PORT_P, 0);            // Create Salvo events.

    __enable_interrupt();               // Required: TimerA0 ISR calls OSTimer.

    while (1) {
      OSSched();                        // Start multitasking.
    }
}
