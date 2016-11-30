/******************************************************************************
(C) Copyright Pumpkin, Inc. All Rights Reserved.

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\Library\\MSP430\\Src\\sw_UART4_msg.c,v $
$Author: aek $
$Revision: 3.1 $
$Date: 2009-10-11 19:48:23-07 $

******************************************************************************/
#include "config.h"
#include "sw_uart_msg.h"
#include "sw_uart.h"


/******************************************************************************
****                                                                       ****
**                                                                           **
sw_uart4_msg()

Output a message.

**                                                                           **
****                                                                       ****
******************************************************************************/
void sw_uart4_msg(const char * cP) {

  unsigned int size;


  size = strlen(cP) + sizeof(CRLF);

  if (size < TX4_BUFF_SIZE) {
    while (sw_uart4_tx_free() < size);
    sw_uart4_puts(cP);
    sw_uart4_puts(CRLF);
  }
  
} /* sw_uart4_msg() */

