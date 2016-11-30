/******************************************************************************
(C) Copyright Pumpkin, Inc. All Rights Reserved.

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\Library\\MSP430\\Src\\USCI_A1_UART_msg_ts.c,v $
$Author: aek $
$Revision: 3.2 $
$Date: 2009-11-01 22:53:48-08 $

******************************************************************************/
#include "config.h"
#include "usci_a_uart_msg_ts.h"
#include "usci_a_uart.h"
#include <salvo.h>


/******************************************************************************
****                                                                       ****
**                                                                           **
usci_a1_uart_msg_ts()

Output a time-stamped message. 

**                                                                           **
****                                                                       ****
******************************************************************************/
void usci_a1_uart_msg_ts(const char * cP) {

  unsigned int size;
  char strTicks[TICKS_BUFFER_SIZE];
  

  sprintf(strTicks, "1:%010lu ", OSGetTicks());

  size = 1 + sizeof(strTicks) + strlen(cP) + sizeof(CRLF);

  if (size < TX1_BUFF_SIZE) {
    while (usci_a1_uart_tx_free() < size);
    usci_a1_uart_puts(strTicks);
    usci_a1_uart_puts(cP);
    usci_a1_uart_puts(CRLF);
  } /* if */
} /* usci_a1_uart_msg_ts */

