/******************************************************************************
(C) Copyright Pumpkin, Inc. All Rights Reserved.

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\Library\\PIC24\\Src\\pic24_uart1_msg_ts.c,v $
$Author: aek $
$Revision: 3.1 $
$Date: 2014-06-02 08:59:59-07 $

******************************************************************************/
#include "config.h"
#include "pic24_uart_msg_ts.h"
#include "pic24_uart.h"
#include <salvo.h>
#include <string.h>


/******************************************************************************
****                                                                       ****
**                                                                           **
uart1_msg_ts()

Output a time-stamped message.

**                                                                           **
****                                                                       ****
******************************************************************************/
void uart1_msg_ts(const char * cP) {

  unsigned int size;
  char strTicks[TICKS_BUFFER_SIZE];
  
  sprintf(strTicks, "0:%010lu ", OSGetTicks());

  size = 1 + sizeof(strTicks) + strlen(cP) + sizeof(CRLF);

  if (size < TX1_BUFF_SIZE) {
    while (uart1_tx_free() < size);
    uart1_puts(strTicks);
    uart1_puts(cP);
    uart1_puts(CRLF);
  } /* if */
  
} /* uart1_msg_ts() */

