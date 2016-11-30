/******************************************************************************
(C) Copyright Pumpkin, Inc. All Rights Reserved.

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\Library\\MSP430\\Src\\sw_UART4_msg_ts.c,v $
$Author: aek $
$Revision: 3.2 $
$Date: 2009-11-01 22:53:50-08 $

******************************************************************************/
#include "config.h"
#include "sw_uart_msg_ts.h"
#include "sw_uart.h"
#include <salvo.h>


/******************************************************************************
****                                                                       ****
**                                                                           **
sw_uart4_msg_ts()

Output a time-stamped message.

**                                                                           **
****                                                                       ****
******************************************************************************/
void sw_uart4_msg_ts(const char * cP) {

  unsigned int size;
  char strTicks[TICKS_BUFFER_SIZE];
  

  sprintf(strTicks, "1:%010lu ", OSGetTicks());

  size = 1 + sizeof(strTicks) + strlen(cP) + sizeof(CRLF);

  if (size < TX4_BUFF_SIZE) {
    while (sw_uart4_tx_free() < size);
    sw_uart4_puts(strTicks);
    sw_uart4_puts(cP);
    sw_uart4_puts(CRLF);
  }

} /* sw_uart4_msg_ts() */
