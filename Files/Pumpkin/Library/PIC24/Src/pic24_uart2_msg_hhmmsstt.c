/******************************************************************************
(C) Copyright Pumpkin, Inc. All Rights Reserved.

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\Library\\PIC24\\Src\\pic24_uart2_msg_hhmmsstt.c,v $
$Author: aek $
$Revision: 3.0 $
$Date: 2014-03-17 17:50:20-08 $

******************************************************************************/
#include "config.h"
#include "pic24_uart_msg_hhmmsstt.h"
#include "pic24_uart.h"
#include <salvo.h>
#include <string.h>

#include "salvotime.h"


/******************************************************************************
****                                                                       ****
**                                                                           **
uart2_msg_hhmmsstt()

Output a message with a hh:mm:ss:tt timestamp.

**                                                                           **
****                                                                       ****
******************************************************************************/
void uart2_msg_hhmmsstt(const char * cP) 
{
  unsigned int size;
  char strTicks[HHMMSSTT_BUFFER_SIZE];
  

  sprintf(strTicks, "1)%s ", OSElapsedTime());

  size = 1 + sizeof(strTicks) + strlen(cP) + sizeof(CRLF);

  if (size < TX2_BUFF_SIZE) {
    while (uart2_tx_free() < size);
    uart2_puts(strTicks);
    uart2_puts(cP);
    uart2_puts(CRLF);
  } /* if */
  
} /* uart2_msg_hhmmsstt() */

