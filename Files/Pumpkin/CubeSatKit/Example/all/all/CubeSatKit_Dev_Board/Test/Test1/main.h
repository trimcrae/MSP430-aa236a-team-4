/******************************************************************************
(C) Copyright Pumpkin, Inc. All Rights Reserved.

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\CubeSatKit\\Example\\all\\all\\CubeSatKit_Dev_Board\\Test\\Test1\\main.h,v $
$Author: aek $
$Revision: 3.8 $
$Date: 2011-09-17 22:12:29-07 $

******************************************************************************/
#ifndef __main_h
#define __main_h


// Symbols, etc.
#define STR_APP_NAME          "Test\\Test1 application"
#define STR_VERSION           "v1.2.4" " built on " __DATE__ " at " __TIME__
#define STR_WARNING           "WARNING: Use 'z' command with caution -- refer to project's abstract.txt"
#define STR_BAUD_RATE         "9600"
#define STR_1TAB              "\t"
#define STR_2TABS             "\t\t"
#define STR_CRLF              "\r\n"
  
// Macros for user-readable messages.
#define user_debug_msg(x)     csk_uart0_msg_ts(x)
#define data_debug_msg(x)     csk_uart1_msg_ts(x)

// Other macros.
#define LOOP_HERE()            do { ; } while (1)

// Target-specific symbols.
#define SYSTEM_TIMER_RELOAD    328
#define NOP                    _NOP()

// Function declarations.
extern void init_devices(void);

// Structure declarations  
typedef struct {
  unsigned int usb_present:1;
  unsigned int usb_connected:1;
  unsigned int mhx_connected:1;
  unsigned int MCLKOutEnabled:2;
  unsigned int exercise_io_running:1;
} csk_status_t;

// Extern variable declarations.
extern csk_status_t csk_status;
extern char strTmp[];


#endif /* __main_h */






