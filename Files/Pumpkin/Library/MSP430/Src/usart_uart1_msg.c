/******************************************************************************
(C) Copyright Pumpkin, Inc. All Rights Reserved.

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\Library\\MSP430\\Src\\USART_UART1_msg.c,v $
$Author: aek $
$Revision: 3.3 $
$Date: 2009-10-11 19:48:26-07 $

******************************************************************************/
#include "config.h"
#include "usart_uart_msg.h"
#include "usart_uart.h"


/******************************************************************************
****                                                                       ****
**                                                                           **
usart_uart1_msg()

Output a message.
**                                                                           **
****                                                                       ****
******************************************************************************/
void usart_uart1_msg(const char * cP)
{
  unsigned int size;

  // Let's find out how big this string is ...
  size = strlen(cP) + sizeof(CRLF);

  // Only if it can theoretically fit AND there's room in
  //  the buffer do we output it.
  if (size < TX1_BUFF_SIZE) {
    while (usart_uart1_tx_free() < size);
    usart_uart1_puts(cP);
    usart_uart1_puts(CRLF);
  }
  
  // Else handle error.
  
} /* usart_uart1_msg() */
