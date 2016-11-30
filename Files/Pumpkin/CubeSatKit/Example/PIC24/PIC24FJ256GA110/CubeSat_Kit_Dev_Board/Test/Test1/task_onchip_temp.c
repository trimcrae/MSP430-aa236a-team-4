/******************************************************************************
(C) Copyright Pumpkin, Inc. All Rights Reserved.

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\CubeSatKit\\Example\\PIC24\\PIC24FJ256GA110\\CubeSat_Kit_Dev_Board\\Test\\Test1\\task_onchip_temp.c,v $
$Author: aek $
$Revision: 3.1 $
$Date: 2010-02-13 11:20:03-08 $

******************************************************************************/
#include "main.h"
#include "task_onchip_temp.h"

// Pumpkin CubeSat Kit headers
#include "csk_uart.h"

// Pumpkin Salvo headers
#include "salvo.h"

// Private variables.
static unsigned int deg_c = 0;


/******************************************************************************
****                                                                       ****
**                                                                           **
void task_onchip_temp()

Since the PIC24FJ256GA110 does not have an on-chip temperature sensor, this
task does nothing ...

**                                                                           **
****                                                                       ****
******************************************************************************/
void task_onchip_temp(void) {

  user_debug_msg(STR_TASK_ONCHIP_TEMP "Stopped.");

  OS_Stop();

} /* task_onchip_temp */


/******************************************************************************
****                                                                       ****
**                                                                           **
unsigned int onchip_temp_deg_c()

Return on-chip temp in degrees C.

**                                                                           **
****                                                                       ****
******************************************************************************/
unsigned int onchip_temp_deg_c(void) {
  return deg_c;
}
