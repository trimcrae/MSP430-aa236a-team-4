/******************************************************************************
(C) Copyright Pumpkin, Inc. All Rights Reserved.

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\CubeSatKit\\Example\\all\\all\\CubeSatKit_Dev_Board\\SD\\SD1\\task_sd_append.c,v $
$Author: aek $
$Revision: 3.7 $
$Date: 2011-03-23 20:49:55-08 $

******************************************************************************/
#include "main.h"
#include "events.h"
#include "init.h"
#include "task_sd_append.h"

// Pumpkin CubeSat Kit headers
#include "csk_sd.h"
#include "csk_uart.h"

// HCC-Embedded + CubeSat Kit headers
#include "thin_usr.h"

// Pumpkin Salvo headers
#include "salvo.h"

#define STR_TO_APPEND       "Iterations: %010lu  Current ticks: %010lu.\r\n"


/******************************************************************************
****                                                                       ****
**                                                                           **
task_sd_append()

Work the SD Card.

**                                                                           **
****                                                                       ****
******************************************************************************/
void task_sd_append(void) {

  F_FILE *file;
  unsigned int length;
  static unsigned long count_appends;
  
  
  user_debug_msg(STR_TASK_SD_APPEND "Starting.");

  // Here is where we init the SD Card.  Since RSRC_SD_CARD_P is initially
  //  0 (i.e., not available), no task that waits on it can proceed.
  csk_sd_pwr_on();
  csk_sd_open();
  OS_Delay(100);
  f_initvolume();

  // Now we make the SD Card available to all tasks.
  OSSignalBinSem(RSRC_SD_CARD_P);

  // Reset counter.
  count_appends = 0;
  
  while (1) {
  
    // Must wait until SD Card resource is available.
    OS_WaitBinSem(RSRC_SD_CARD_P, OSNO_TIMEOUT);
    
    // SD Card resource has been successfully acquired.
  
    // Attempt to open the file, in append mode.
    file = f_open(STR_FILE_NAME, "a+");
    
    // If successful, append a new line to the file and report to user.
    if (file) {

      // Place banner at start of each new run.
      // !!strTmp[] must be large enough to handle this string !!
      if (count_appends == 0) {
        sprintf(strTmp, STR_CRLF "Pumpkin " STR_CSK_TARGET " " STR_APP_NAME "." 
                 STR_CRLF STR_VERSION "." 
                 STR_CRLF STR_TO_APPEND, count_appends, OSGetTicks());
      }
      // Always log counter and current ticks.
      else {
        sprintf(strTmp, STR_TO_APPEND, count_appends, OSGetTicks());
      }   
      if ((length = f_write(strTmp, 1, strlen(strTmp), file))) {
        sprintf(strTmp, STR_TASK_SD_APPEND "Appended %d bytes to " STR_FILE_NAME ".", length); 
        user_debug_msg(strTmp);
        count_appends++;
      }
      else {
        user_debug_msg(STR_TASK_SD_APPEND "f_write failed to write to " STR_FILE_NAME ".");
      }
    }
    // O/wise flag as an error.
    else {
      user_debug_msg(STR_TASK_SD_APPEND "Cannot open " STR_FILE_NAME ".");
    }
    
    // Done, close file, release SD Card, repeat after 2s.
    f_close(file);

    OSSignalBinSem(RSRC_SD_CARD_P);
    OS_Delay(200);
    
  } /* while */
  
} /* task_sd_append */
