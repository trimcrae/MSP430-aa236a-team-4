/******************************************************************************
(C) Copyright Pumpkin, Inc. All Rights Reserved.

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\Library\\PIC24\\Src\\pic24_uart4_msg_hhmmsstt.c,v $
$Author: aek $
$Revision: 3.0 $
$Date: 2014-03-17 17:50:19-08 $

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
uart3_msg_hhmmsstt()

Output a message with a hh:mm:ss:tt timestamp.

**                                                                           **
****                                                                       ****
******************************************************************************/
void uart3_msg_hhmmsstt(const char * cP) 
{
  unsigned int size;
  char strTicks[HHMMSSTT_BUFFER_SIZE];
  

  sprintf(strTicks, "2)%s ", OSElapsedTime());

  size = 1 + sizeof(strTicks) + strlen(cP) + sizeof(CRLF);

  if (size < TX2_BUFF_SIZE) {
    while (uart3_tx_free() < size);
    uart3_puts(strTicks);
    uart3_puts(cP);
    uart3_puts(CRLF);
  } /* if */
  
} /* uart3_msg_hhmmsstt() */

