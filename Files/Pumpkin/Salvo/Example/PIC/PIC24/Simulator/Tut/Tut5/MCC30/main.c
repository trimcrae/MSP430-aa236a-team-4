/************************************************************ 
Copyright (C) 1995-2006 Pumpkin, Inc. and its
Licensor(s). Freely distributable.

$Source: C:\\RCS\\D\\Pumpkin\\Salvo\\Example\\PIC\\PIC24\\Simulator\\Tut\\Tut5\\MCC30\\main.c,v $
$Author: aek $
$Revision: 3.0 $
$Date: 2007-06-27 13:29:22-07 $

Tutorial program Tut5.

See Salvo User Manual for in-depth description. 

************************************************************/

#include <salvo.h>
#include "p24HJ256GP610.h"

#define LOOPS_PER_TICK			 100		/* <64K			  */

#define TASK_COUNT_P             OSTCBP(1) 	/* task #1        */
#define TASK_SHOW_P              OSTCBP(2) 	/*  ""  #2        */
#define TASK_BLINK_P             OSTCBP(3) 	/*  ""  #3        */

#define PRIO_COUNT               10        	/* task priorities*/
#define PRIO_SHOW                10        	/*  ""            */
#define PRIO_BLINK               2         	/*  ""            */

#define BINSEM_UPDATE_PORT_P     OSECBP(1) 	/* binSem #1      */

unsigned int counter;


void TaskCount (void)
{
    for (;;) 
    {
        counter++;
        
        /* Every time bit 8 of counter rolls over, signal the binSem. */ 
        if (!(counter & 0x01FF))
        {
            OSSignalBinSem(BINSEM_UPDATE_PORT_P);
        }
    
        /* Context-switch so other tasks can run.                     */
        OS_Yield();
    }
}


void TaskShow (void)
{
    for (;;) 
    {
        /* Context-switch and wait for TaskCount() to signal that 512 */
        /*  counts have elapsed.                                      */
        OS_WaitBinSem(BINSEM_UPDATE_PORT_P, OSNO_TIMEOUT);
        
        /* Write upper 7 bits of 16-bit counter PORTA:[7..1].         */
		PORTA  = (PORTA & ~0xFE) | ((counter >> 8) & 0xFE);
    }
}


void TaskBlink (void)
{
    /* Set PORTA to all-outputs, initialize to 0x00. This only happens  	*/
    /*  when the tasks starts.                                            	*/
    TRISA = 0x00; 
	PORTA = 0xAA;
	LATA  = 0xAA;
           

    for (;;) 
    {
        /* Toggle PORTA:0													*/
		PORTA ^= 0x01;			
        
        /* Context-switch and return in 50 ticks.							*/
        OS_Delay(50); 							
    }
}


int main (void)
{
 	unsigned int ticks;
	

    OSInit();
    
    OSCreateTask(TaskCount, TASK_COUNT_P, PRIO_COUNT);
    OSCreateTask(TaskShow,  TASK_SHOW_P,  PRIO_SHOW );
    OSCreateTask(TaskBlink, TASK_BLINK_P, PRIO_BLINK);
    
    OSCreateBinSem(BINSEM_UPDATE_PORT_P, 0);
    
    counter = 0;
	ticks = LOOPS_PER_TICK;
	
	__asm__ volatile("disi #0x0000");
    
    for (;;)
    {
	  /* synthesize a system tick rate without having to use interrupts.	*/
	  if (ticks-- == 0)
	  {
	    OSTimer();
		ticks = LOOPS_PER_TICK;
	  }

      OSSched();
    }
}
