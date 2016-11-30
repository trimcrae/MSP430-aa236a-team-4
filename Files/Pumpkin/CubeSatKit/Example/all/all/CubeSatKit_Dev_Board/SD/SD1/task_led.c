/******************************************************************************
(C) Copyright Pumpkin, Inc. All Rights Reserved.

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\CubeSatKit\\Example\\all\\all\\CubeSatKit_Dev_Board\\SD\\SD1\\task_led.c,v $
$Author: aek $
$Revision: 3.3 $
$Date: 2011-03-23 20:49:55-08 $

******************************************************************************/
#include "main.h"
#include "init.h"
#include "task_led.h"

// Pumpkin CubeSat Kit headers
#include "csk_led.h"
#include "csk_uart.h"

// Pumpkin Salvo headers
#include "salvo.h"


/******************************************************************************
****                                                                       ****
**                                                                           **
task_led()

Simple task that flashes the CSK's LED (when present).

**                                                                           **
****                                                                       ****
******************************************************************************/
void task_led(void) {

  user_debug_msg(STR_TASK_LED "Starting.");
  csk_led_status_open();  

  while (1) {
    
    // Flash LED -- on time is 1/10 the off time.
    csk_led_status_off();
    OS_Delay(50);
    csk_led_status_on();
    OS_Delay(5);  

  } /* while */

} /* task_led() */


