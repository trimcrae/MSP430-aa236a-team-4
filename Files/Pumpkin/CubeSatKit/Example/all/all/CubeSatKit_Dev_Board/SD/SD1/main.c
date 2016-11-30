/******************************************************************************
(C) Copyright Pumpkin, Inc. All Rights Reserved.

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\CubeSatKit\\Example\\all\\all\\CubeSatKit_Dev_Board\\SD\\SD1\\main.c,v $
$Author: aek $
$Revision: 3.4 $
$Date: 2010-04-24 23:26:26-07 $

******************************************************************************/
#include "main.h"
#include "init.h"
#include "events.h"
#include "task_led.h"
#include "task_sd_append.h"
#include "task_sd_measure.h"
#include "tasks.h"

// Pumpkin Salvo headers
#include "salvo.h"

char strTmp[256];  // Usable to anyone who wants it ...


/******************************************************************************
****                                                                       ****
**                                                                           **
main()

Conventional Salvo main(), with peripheral initialization, Salvo 
initialization, and the Salvo scheduler.

**                                                                           **
****                                                                       ****
******************************************************************************/
int main(void) {

  // Do target-specific (e.g., clocks, UARTs) and general 
  //  (e.g., CSK IO) initialization.
  init();
                              
  // Initialize Salvo RTOS.
  OSInit();

  // Create tasks.
  OSCreateTask(task_led,        TASK_LED_P,          3);
  OSCreateTask(task_sd_append,  TASK_SD_APPEND_P,   11);
  OSCreateTask(task_sd_measure, TASK_SD_MEASURE_P,   7);
  
  // Create events.
  OSCreateBinSem(RSRC_SD_CARD_P, 0);    // Initially unavailable.

  // Enable interrupts (enables UART tx & rx).
  __enable_interrupt();

  // Multitask.
  while (1) {
    OSSched();
  } /* while */
  
} /* main() */
