/******************************************************************************
(C) Copyright Pumpkin, Inc. All Rights Reserved.

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\Library\\MSP430\\Src\\USART_UART1_msg_ts.c,v $
$Author: aek $
$Revision: 3.2 $
$Date: 2009-11-01 22:53:51-08 $

******************************************************************************/
#include "config.h"
#include "usart_uart_msg_ts.h"
#include "usart_uart.h"
#include <salvo.h>


/******************************************************************************
****                                                                       ****
**                                                                           **
usart_uart1_msg_ts()

Output a time-stamped message.

**                                                                           **
****                                                                       ****
******************************************************************************/
void usart_uart1_msg_ts(const char * cP) {

  unsigned int size;
  char strTicks[TICKS_BUFFER_SIZE];
  
  // Format strTicks to indicate which USART is talking, and display a timestamp.
  sprintf(strTicks, "1:%010lu ", OSGetTicks());

  // Let's find out how big this string is ...
  size = 1 + sizeof(strTicks) + strlen(cP) + sizeof(CRLF);

  // Only if it can theoretically fit AND there's room in
  //  the buffer do we output it.
  if (size < TX1_BUFF_SIZE) {
    while (usart_uart1_tx_free() < size);
    usart_uart1_puts(strTicks);
    usart_uart1_puts(cP);
    usart_uart1_puts(CRLF);
  } /* if */
  
  // Else handle error.

} /* usart_uart1_msg_ts() */

