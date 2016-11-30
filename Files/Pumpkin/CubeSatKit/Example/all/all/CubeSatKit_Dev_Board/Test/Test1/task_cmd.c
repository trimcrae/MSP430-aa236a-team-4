/******************************************************************************
(C) Copyright Pumpkin, Inc. All Rights Reserved.

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\CubeSatKit\\Example\\all\\all\\CubeSatKit_Dev_Board\\Test\\Test1\\task_cmd.c,v $
$Author: aek $
$Revision: 3.4 $
$Date: 2010-02-13 13:17:57-08 $

******************************************************************************/
#include "main.h"
#include "cmd.h"
#include "events.h"
#include "task_cmd.h"
#include "task_onchip_temp.h"
#include "tasks.h"

// C headers
#include <ctype.h>

// Pumpkin CubeSat Kit headers
#include "csk_power.h"
#include "csk_uart.h"
#include "csk_wdt.h"

// Pumpkin Salvo headers
#include "salvo.h"


/******************************************************************************
****                                                                       ****
**                                                                           **
cmd_explain()
task_cmd_do()

Act on single-character commands:

  h|?: help
  i:   toggle Exercise I/O task on/off
  m:   cycle between various MCLK output settings
  r:   restart application
  t:   display on-chip temperature
  v:   display version number
  z:   go to sleep
 
**                                                                           **
****                                                                       ****
******************************************************************************/
static
void cmd_explain(void) {

  user_debug_msg(STR_CMD_EXPLAIN "h|?: Commands: {h|?, i, m, r, t, v, z}");

} /* cmd_explain() */


void task_cmd_do(void) {

  unsigned char cmd;

  
  user_debug_msg(STR_TASK_CMD_DO "Starting.");
  OS_Delay(50);
  
  csk_status.MCLKOutEnabled = 0;
  
  // Intro messages. Buffer is pretty small, so wait a bit to flush it.
  user_debug_msg(STR_TASK_CMD_DO "Control via RS-232 I/F at " STR_BAUD_RATE ",N,8,1.");
  OS_Delay(50);

  user_debug_msg(STR_TASK_CMD_DO "Microhard MHX modem at " STR_BAUD_RATE ",N,8,1.");
  OS_Delay(50);

  cmd_explain();
  
  while (1) {

    OS_WaitSem(SEM_CMD_CHAR_P, OSNO_TIMEOUT);

    if ((cmd=csk_uart0_getchar())) {

      switch (tolower(cmd)) {
      
        // Help.
        case 'h':
        case '?':
          cmd_explain();
          break;


        // Start/stop the exercising of the I/O.
        case 'i':
          if (csk_status.exercise_io_running == FALSE) {
            OSStartTask(TASK_EXERCISE_IO_P);
            csk_status.exercise_io_running = TRUE;
            user_debug_msg(STR_TASK_CMD_DO "i: Exercise I/O enabled.");
          } 
          else {
            OSStopTask(TASK_EXERCISE_IO_P);
            csk_status.exercise_io_running = FALSE;
            user_debug_msg(STR_TASK_CMD_DO "i: Exercise I/O disabled.");
          }
          break;


        // Toggle MCLCK output for frequency & duty-cycle check.
        //  This is a highly target-dependent function.
        case 'm':
          cmd_mclk();
          break;
        

        // Reset.
        case 'r':
          user_debug_msg(STR_TASK_CMD_DO "r: Reset in 1 s.");
          OS_Delay(100);
          csk_wdt_force();
          break;


        // Read on-chip temp sensor.
        case 't':
          sprintf(strTmp, STR_TASK_CMD_DO "t: Uncalibrated on-chip temperature is %dC.", onchip_temp_deg_c());
          user_debug_msg(strTmp);
          break;


        // Tell version.
        case 'v':
          user_debug_msg(STR_TASK_CMD_DO "v: " STR_VERSION ".");
          break;


        // Shut down PPM MCU, for static current measurement.         
        case 'z':
          user_debug_msg(STR_TASK_CMD_DO "z: Full shutdown in 2 s.");

          // Stop all tasks but this one.  OK to use <cmd> since we'll sleep before
          //  exiting this case.
          for (cmd=2; cmd<=OSTASKS; cmd++) {
            OSStopTask(OSTCBP(cmd));
          }

          // Wait a while to flush RS-232, etc.
          OS_Delay(100);

          // Final message.
          user_debug_msg(STR_TASK_CMD_DO "z: System is now shut down.");
          OS_Delay(100);

          // Shut everything down.
          csk_power_shutdown();
          break;


        default:
          sprintf(strTmp, STR_TASK_CMD_DO "Unknown command: '%c' (0x%X)", cmd, cmd);
          user_debug_msg(strTmp);
          break;

      } /* switch */
      
    } /* if */
    
  } /* while */
  
} /* task_cmd_do() */

