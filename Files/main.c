#include <__cross_studio_io.h>    // for CrossStudio debugging

#include "adc.h"                  // Req'd because we reference TaskADC()
#include "batt.h"                 // Req'd because we reference TaskBatt()
#include "init.h"                 // Req'd because we call Init()
#include "io2.h"                   // Req'd because we reference TaskIO()
#include "main.h"                 // Application header
#include "salvo.h"                // Req'd because we call e.g. OSInit() 
#include "usart_uart.h"           // Req'd because we call usart_uart1_puts()
#include "usart_uart_msg_ts.h"    // Req'd because we call usart_uart1_msg_ts()
#include "ui.h"


/******************************************************************************
****                                                                       ****
**                                                                           **
Global variables used by any module in the application
**                                                                           **
****                                                                       ****
******************************************************************************/
char strTmp[256];  // General-purpose buffer for creating long strings


/******************************************************************************
****                                                                       ****
**                                                                           **
Here is a place to put doxygen-compatible comments & tags, etc. for main()

**                                                                           **
****                                                                       ****
******************************************************************************/
void main(void) {

  Init();

  // Now that the non-RTOS stuff has been initialized, we can output via USB.
  usart_uart1_puts("\r\n");
  usart_uart1_msg_ts("main: SSDL Team XXX AA236A 1617 Homework Assignment #2");
  usart_uart1_msg_ts("  v" VERSION_NUM " built on "__DATE__" at "__TIME__"."); // note indent of two spaces

  // Initialize the RTOS.
  OSInit();

  // Create the various tasks.
  OSCreateTask(TaskADC,        OSTCBP(1),  3);
  OSCreateTask(TaskBatt,       OSTCBP(2),  4);
  OSCreateTask(TaskIO,         OSTCBP(3),  11);
  OSCreateTask(TaskDoCmds,     OSTCBP(4),  12);

  // Since ISRs are present, we must enable interrupts globally. No need to touch interrupts again.
  __enable_interrupt();
  setChargerOn();
  setProgOn();

  // OK to use, but keep in mind that it has weird run-time effects and requires that the FET
  //  be present
//  debug_printf("main: This is debug output in the CrossWorks IDE Debug I/O Terminal.\n");

  // Run the scheduler open-loop for best performance.
  while(1) {
    OSSched();
  }

  // We never reach this.
  debug_exit(0);
}


