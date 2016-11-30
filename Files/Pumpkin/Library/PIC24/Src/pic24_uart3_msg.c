/******************************************************************************
(C) Copyright Pumpkin, Inc. All Rights Reserved.

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\Library\\PIC24\\Src\\pic24_uart3_msg.c,v $
$Author: aek $
$Revision: 3.0 $
$Date: 2014-03-17 17:50:22-08 $

******************************************************************************/
#include "config.h"
#include "pic24_uart_msg.h"
#include "pic24_uart.h"
#include <string.h>


/******************************************************************************
****                                                                       ****
**                                                                           **
usart_uart0_msg()

Output a message.

**                                                                           **
****                                                                       ****
******************************************************************************/
void uart3_msg(const char * cP) {

  unsigned int size;


  size = strlen(cP) + sizeof(CRLF);

  if (size < TX1_BUFF_SIZE) {
    while (uart3_tx_free() < size);
    uart3_puts(cP);
    uart3_puts(CRLF);
  }
  
} /* usart_uart3_msg() */

