#include "task_repeat.h"              // Good to self-reference
#include "main.h"                 // Application header
#include "msp430.h"               // Contains MSP430-specific definitions like P2DIR
#include "salvo.h"                // Req'd because we call e.g. OSDelay() 
#include "usart_uart_msg_ts.h"    // Req'd because we call usart_uart1_msg_ts()

/******************************************************************************
****                                                                       ****
**                                                                           **

**                                                                           **
****                                                                       ****
******************************************************************************/
void task_repeat(void) {
  usart_uart1_puts("\r\n");
  usart_uart1_msg_ts("AA236A 1415 HA #2 built on "__DATE__" at "__TIME__".");
  
  while(1) {
   usart_uart1_msg_ts("task_repeat: Running.");
   OS_Delay(250);
   OS_Delay(250);
  }
}