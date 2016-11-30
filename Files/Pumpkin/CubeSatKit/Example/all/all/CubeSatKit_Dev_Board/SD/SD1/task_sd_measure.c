/******************************************************************************
(C) Copyright Pumpkin, Inc. All Rights Reserved.

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\CubeSatKit\\Example\\all\\all\\CubeSatKit_Dev_Board\\SD\\SD1\\task_sd_measure.c,v $
$Author: aek $
$Revision: 3.3 $
$Date: 2010-04-24 23:26:27-07 $

******************************************************************************/
#include "main.h"
#include "events.h"
#include "init.h"
#include "task_sd_measure.h"

// Pumpkin CubeSat Kit headers
#include "csk_rand.h"
#include "csk_sd.h"
#include "csk_uart.h"

// HCC-Embedded + CubeSat Kit headers
#include "thin_usr.h"

// Pumpkin Salvo headers
#include "salvo.h"


/******************************************************************************
****                                                                       ****
**                                                                           **
task_sd_measure()

Report on the size of the CSK_TEMP.TXT file on the card every few seconds.

**                                                                           **
****                                                                       ****
******************************************************************************/
void task_sd_measure(void) {

  F_FILE * file;
  unsigned long size;


  user_debug_msg(STR_TASK_SD_MEASURE "Starting.");

  while (1) {
  
    // Wait 2.5s + some random time. Avoid 
    //  OS_Delay(0) because that will stop this task.
    OS_Delay(250); 
    OS_Delay(1 + ((csk_rand()>>8)& 0x007F));
    
    // Acquire SD Card.
    OS_WaitBinSem(RSRC_SD_CARD_P, OSNO_TIMEOUT);
    
    // Open the file, gets its length and report to user.
    file = f_open(STR_FILE_NAME, "r");
    if (file) {
      size = f_filelength(STR_FILE_NAME);
      sprintf(strTmp, STR_TASK_SD_MEASURE STR_FILE_NAME " is %lu bytes long.", size);
      user_debug_msg(strTmp);
    }
    else {
      user_debug_msg(STR_TASK_SD_MEASURE "Cannot open " STR_FILE_NAME ".");
    }
    
    // Done, release SD Card, repeat.
    f_close(file);
    OSSignalBinSem(RSRC_SD_CARD_P);
    
  } /* while */

} /* task_sd_measure */

