/******************************************************************************
(C) Copyright Pumpkin, Inc. All Rights Reserved.

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\Library\\MSP430\\Src\\USART_UART0_msg_ts.c,v $
$Author: aek $
$Revision: 3.2 $
$Date: 2009-11-01 22:53:50-08 $

******************************************************************************/
#include "config.h"
#include "usart_uart_msg_ts.h"
#include "usart_uart.h"
#include <salvo.h>


/******************************************************************************
****                                                                       ****
**                                                                           **
usart_uart0_msg_ts()

Output a time-stamped message.

**                                                                           **
****                                                                       ****
******************************************************************************/
void usart_uart0_msg_ts(const char * cP) {

  unsigned int size;
  char strTicks[TICKS_BUFFER_SIZE];
  

  sprintf(strTicks, "0:%010lu ", OSGetTicks());

  size = 1 + sizeof(strTicks) + strlen(cP) + sizeof(CRLF);

  if (size < TX0_BUFF_SIZE) {
    while (usart_uart0_tx_free() < size);
    usart_uart0_puts(strTicks);
    usart_uart0_puts(cP);
    usart_uart0_puts(CRLF);
  } /* if */
  
} /* usart_uart0_msg_ts() */

