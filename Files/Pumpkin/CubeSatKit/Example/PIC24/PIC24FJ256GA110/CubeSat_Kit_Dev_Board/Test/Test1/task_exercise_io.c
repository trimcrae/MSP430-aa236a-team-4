/******************************************************************************
(C) Copyright Pumpkin, Inc. All Rights Reserved.

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\CubeSatKit\\Example\\PIC24\\PIC24FJ256GA110\\CubeSat_Kit_Dev_Board\\Test\\Test1\\task_exercise_io.c,v $
$Author: aek $
$Revision: 3.2 $
$Date: 2010-02-13 11:20:02-08 $

******************************************************************************/
#include "main.h"
#include "task_exercise_io.h"


// Pumpkin CubeSat Kit headers
#include "csk_io.h"
#include "csk_uart.h"

// Pumpkin Salvo headers
#include "salvo.h"


#define LED_ON_TIME             20  // in ticks

/******************************************************************************
****                                                                       ****
**                                                                           **
task_exercise_io()

**                                                                           **
****                                                                       ****
******************************************************************************/
void task_exercise_io(void) {

  user_debug_msg(STR_TASK_EXERCISE_IO "Starting.");

  // Do all operations inside loop so that when task is restarted, all 
  //  initializations, etc. are performed again.
  while (1) {
  
     // Global flag.
    csk_status.exercise_io_running = 1;
    
    // Signals that may interfere with normal operation (e.g. Tx/Rx) are
    //  left alone ...  
    csk_io0_high();  OS_Delay(LED_ON_TIME); 
    csk_io1_high();  OS_Delay(LED_ON_TIME);  
    csk_io2_high();  OS_Delay(LED_ON_TIME);  
    csk_io3_high();  OS_Delay(LED_ON_TIME);  
  //csk_io4_high();  OS_Delay(LED_ON_TIME);    // TX0
  //csk_io5_high();  OS_Delay(LED_ON_TIME);    // RX0
  //csk_io6_high();  OS_Delay(LED_ON_TIME);    // TX1
  //csk_io7_high();  OS_Delay(LED_ON_TIME);    // RX1
    csk_io8_high();  OS_Delay(LED_ON_TIME);  
    csk_io9_high();  OS_Delay(LED_ON_TIME);  
    csk_io10_high(); OS_Delay(LED_ON_TIME);  
    csk_io11_high(); OS_Delay(LED_ON_TIME);  
    csk_io12_high(); OS_Delay(LED_ON_TIME);  
    csk_io13_high(); OS_Delay(LED_ON_TIME);  
    csk_io14_high(); OS_Delay(LED_ON_TIME); 
    csk_io15_high(); OS_Delay(LED_ON_TIME);  
    csk_io16_high(); OS_Delay(LED_ON_TIME);  
    csk_io17_high(); OS_Delay(LED_ON_TIME);  
    csk_io18_high(); OS_Delay(LED_ON_TIME);  
    csk_io19_high(); OS_Delay(LED_ON_TIME);  
    csk_io20_high(); OS_Delay(LED_ON_TIME);  
    csk_io21_high(); OS_Delay(LED_ON_TIME); 
    csk_io22_high(); OS_Delay(LED_ON_TIME);  
    csk_io23_high(); OS_Delay(LED_ON_TIME);  
    csk_io47_high(); OS_Delay(LED_ON_TIME);  
    csk_io46_high(); OS_Delay(LED_ON_TIME);    
    csk_io45_high(); OS_Delay(LED_ON_TIME);   
    csk_io44_high(); OS_Delay(LED_ON_TIME);    
    csk_io43_high(); OS_Delay(LED_ON_TIME);  
    csk_io42_high(); OS_Delay(LED_ON_TIME); 
    csk_io41_high(); OS_Delay(LED_ON_TIME); 
    csk_io40_high(); OS_Delay(LED_ON_TIME);  
    csk_io39_high(); OS_Delay(LED_ON_TIME);   
    csk_io38_high(); OS_Delay(LED_ON_TIME);  
    csk_io37_high(); OS_Delay(LED_ON_TIME);  
    csk_io36_high(); OS_Delay(LED_ON_TIME);  
    csk_io35_high(); OS_Delay(LED_ON_TIME);  
    csk_io34_high(); OS_Delay(LED_ON_TIME); 
    csk_io33_high(); OS_Delay(LED_ON_TIME);  
  //csk_io32_high(); OS_Delay(LED_ON_TIME);    // status LED
    csk_io31_high(); OS_Delay(LED_ON_TIME);  
    csk_io30_high(); OS_Delay(LED_ON_TIME); 
    csk_io29_high(); OS_Delay(LED_ON_TIME);  
    csk_io28_high(); OS_Delay(LED_ON_TIME); 
    csk_io27_high(); OS_Delay(LED_ON_TIME); 
    csk_io26_high(); OS_Delay(LED_ON_TIME);  
    csk_io25_high(); OS_Delay(LED_ON_TIME); 
    csk_io24_high(); OS_Delay(LED_ON_TIME);  

    csk_io0_low();  OS_Delay(LED_ON_TIME);
    csk_io1_low();  OS_Delay(LED_ON_TIME);
    csk_io2_low();  OS_Delay(LED_ON_TIME);
    csk_io3_low();  OS_Delay(LED_ON_TIME);
  //csk_io4_low();  OS_Delay(LED_ON_TIME);
  //csk_io5_low();  OS_Delay(LED_ON_TIME);
  //csk_io6_low();  OS_Delay(LED_ON_TIME);
  //csk_io7_low();  OS_Delay(LED_ON_TIME);
    csk_io8_low();  OS_Delay(LED_ON_TIME);
    csk_io9_low();  OS_Delay(LED_ON_TIME);
    csk_io10_low(); OS_Delay(LED_ON_TIME);
    csk_io11_low(); OS_Delay(LED_ON_TIME);
    csk_io12_low(); OS_Delay(LED_ON_TIME);
    csk_io13_low(); OS_Delay(LED_ON_TIME);
    csk_io14_low(); OS_Delay(LED_ON_TIME);
    csk_io15_low(); OS_Delay(LED_ON_TIME);
    csk_io16_low(); OS_Delay(LED_ON_TIME);
    csk_io17_low(); OS_Delay(LED_ON_TIME);
    csk_io18_low(); OS_Delay(LED_ON_TIME);
    csk_io19_low(); OS_Delay(LED_ON_TIME);
    csk_io20_low(); OS_Delay(LED_ON_TIME);
    csk_io21_low(); OS_Delay(LED_ON_TIME);
    csk_io22_low(); OS_Delay(LED_ON_TIME);
    csk_io23_low(); OS_Delay(LED_ON_TIME);
    csk_io47_low(); OS_Delay(LED_ON_TIME);
    csk_io46_low(); OS_Delay(LED_ON_TIME);
    csk_io45_low(); OS_Delay(LED_ON_TIME);
    csk_io44_low(); OS_Delay(LED_ON_TIME);
    csk_io43_low(); OS_Delay(LED_ON_TIME);
    csk_io42_low(); OS_Delay(LED_ON_TIME);
    csk_io41_low(); OS_Delay(LED_ON_TIME);
    csk_io40_low(); OS_Delay(LED_ON_TIME);
    csk_io39_low(); OS_Delay(LED_ON_TIME);
    csk_io38_low(); OS_Delay(LED_ON_TIME);
    csk_io37_low(); OS_Delay(LED_ON_TIME);
    csk_io36_low(); OS_Delay(LED_ON_TIME);
    csk_io35_low(); OS_Delay(LED_ON_TIME);
    csk_io34_low(); OS_Delay(LED_ON_TIME);
    csk_io33_low(); OS_Delay(LED_ON_TIME);
  //csk_io32_low(); OS_Delay(LED_ON_TIME);
    csk_io31_low(); OS_Delay(LED_ON_TIME);
    csk_io30_low(); OS_Delay(LED_ON_TIME);
    csk_io29_low(); OS_Delay(LED_ON_TIME);
    csk_io28_low(); OS_Delay(LED_ON_TIME);
    csk_io27_low(); OS_Delay(LED_ON_TIME);
    csk_io26_low(); OS_Delay(LED_ON_TIME);
    csk_io25_low(); OS_Delay(LED_ON_TIME);
    csk_io24_low(); OS_Delay(LED_ON_TIME);

  } /* while */

} /* task_exercise_io() */
