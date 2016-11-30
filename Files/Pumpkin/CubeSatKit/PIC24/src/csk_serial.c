/******************************************************************************
(C) Copyright Pumpkin, Inc. All Rights Reserved.

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\CubeSatKit\\PIC24\\Src\\csk_serial.c,v $
$Author: aek $
$Revision: 3.1 $
$Date: 2011-09-17 16:24:48-07 $

******************************************************************************/
#include "csk_serial.h"
#include "csk_uart.h"
#include <stdio.h>


// Defines the serial port for stdin, stdout & stderr
static unsigned char csk_serial_port;


/******************************************************************************
****                                                                       ****
**                                                                           **
write()

This function overrides the low-level write() function in the MPLAB C30 
library, enabling the redirection of e.g. printf() w/regard to stdout, which
is normally directed to UART1.

Adapted from Microchip's write.c in MPLAB C30's standard library. See Microchip
document number Q6UJ9A009MTU.

Unfortunately this function does not seamlessly provide support for simulated
I/O in MPLAB SIM -- that requires Microchip's simio.h, etc.

**                                                                           **
****                                                                       ****
******************************************************************************/
int write(int handle, void *buffer, unsigned int len) {
  int i;


  switch (handle) {
    case 0:
    case 1:
    case 2:
    default:

      for (i=len; i; --i) {
      
        // A specified serial port of 0 goes to UART0, all others go to UART1
        if (!csk_serial_port) {
          csk_uart0_putchar(*(char*)buffer++);
        }
        else {
          csk_uart1_putchar(*(char*)buffer++);
        }
      }
      
      break;  
  }
  return(len);
}


/******************************************************************************
****                                                                       ****
**                                                                           **
csk_serial_stdin_select()

Select which port you want to use (e.g. for printf());

**                                                                           **
****                                                                       ****
******************************************************************************/
void csk_serial_stdin_select(unsigned char port) {

  csk_serial_port = port;
  
} /* serial_stdin_select() */


/******************************************************************************
****                                                                       ****
**                                                                           **
csk_serial_get_byte()

Get a byte from the selected serial port.

**                                                                           **
****                                                                       ****
******************************************************************************/
unsigned char csk_serial_get_byte(unsigned char port, 
                                  unsigned char * data) {

  if (!port) {
  	if (csk_uart0_rx_count() == 0) {
  	  return(0);	
  	}
  	else {
  	 *data = csk_uart0_getchar();
  	}
  }
  else {
  	if (csk_uart1_rx_count() == 0) {
  	  return(0);	
  	}
  	else {
  	 *data = csk_uart1_getchar();
  	}
  }

  // Success
  return(1);

} /* serial_get_byte() */
