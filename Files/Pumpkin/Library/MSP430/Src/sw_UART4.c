/******************************************************************************
(C) Copyright Pumpkin, Inc. All Rights Reserved.

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\Library\\MSP430\\Src\\sw_UART4.c,v $
$Author: aek $
$Revision: 3.2 $
$Date: 2009-11-01 22:53:49-08 $

******************************************************************************/

/******************************************************************************

IMPORTANT NOTE: This module is under construction. Initially,
only software tx is supported.

Assuming that future MSP430s may have up to 4 UARTs (numbered
UART0 through UART3), the software UARTs start at UART4.

******************************************************************************/
#include "config.h"
#include "IO.h"
#include "sw_uart.h"



// Control vars and ring buffers for Rx4.
#if (ENABLE_RX4)
unsigned int  rx4Count;
unsigned int  rx4InP, rx4OutP;
unsigned char rx4Buff[RX4_BUFF_SIZE];
#endif
    
// Control vars and ring buffers for Tx4.
#if (ENABLE_TX4)
unsigned int  tx4Count;
unsigned int  tx4InP, tx4OutP;
unsigned char tx4Buff[TX4_BUFF_SIZE];
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
sw_uart4_open()

Algorithm:  
      
Notes:    Runs only at N,8,1.

Returns:  --      

**                                                                           **
****                                                                       ****
******************************************************************************/
void sw_uart4_open(unsigned int period) {

  #if (ENABLE_RX4)
  rx4Count  =                                   0;
  rx4InP    =                                   0;
  rx4OutP   =                                   0;
  #endif
  #if (ENABLE_TX4)
  tx4Count  =                                   0;
  tx4InP    =                                   0;
  tx4OutP   =                                   0;
  #endif

  sw_uart4_hook_open();
  
} /* sw_uart4_open() */


/******************************************************************************
****                                                                       ****
**                                                                           **
sw_uart4_close()
      
Notes:    Does not check buffers for their current status

Returns:  --      

**                                                                           **
****                                                                       ****
******************************************************************************/
void sw_uart4_close(void) {

  sw_uart4_hook_close();

} /* sw_uart4_close() */


/******************************************************************************
****                                                                       ****
**                                                                           **
sw_uart4_peek_rx()

Peek into rx4 buffer at specified location.

Returns:  character at specified location
          EOF if specified location exceeds the number
            of characters in the buffer.      

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_RX4)
int sw_uart4_peek_rx(unsigned int index) {

  int c;

  
  if (index >= rx4Count) {
    return EOF_RETURN_CODE;
  }  
  else {  
    if ((rx4OutP+index) > RX4_BUFF_SIZE-1) {
      c = rx4Buff[rx4OutP+index-RX4_BUFF_SIZE];
    }
    
    else {
      c = rx4Buff[rx4OutP+index];
    }
  } /* else */
        
  return c;

} /* sw_uart4_peek_rx() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
sw_uart4_rx_count()

Returns:  Number of characters currently in rx4 buffer.      

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_RX4)
unsigned int sw_uart4_rx_count(void) {

  return rx4Count;

} /* sw_uart4_rx_count() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
sw_uart4_tx_free()

Returns:  Maximimum number of characters that can currently be placed 
           into the tx4 buffer.      

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_TX4)
unsigned int sw_uart4_tx_free(void) {

  return TX4_BUFF_SIZE-tx4Count;

} /* sw_uart4_tx_free() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
sw_uart4_getchar()

Removes a character (if present) from the rx4 ring buffer and returns it as 
an int.

Algorithm:  if (char in Rx ring buffer)
              get character from Rx ring buffer
              wrap Rx ring buffer's output ptr if req'd
              decrement Rx ring buffer count
              return character
            else
              return 0  

Notes:     

Returns:    character if a character was present.
            0 if buffer was empty.

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_RX4)
int sw_uart4_getchar(void) {

  int c;


  if (rx4Count) {
    c = rx4Buff[rx4OutP++];

    if (rx4OutP > RX4_BUFF_SIZE-1) {
        rx4OutP = 0;
    }
    
    rx4Count--;

    return c;
  }  
  else {
    return EOF_RETURN_CODE;
  } /* else */

} /* sw_uart4_getchar() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
sw_uart4_putchar()

Puts the character into tx4's transmitter's ring buffer
if room is available.

We enable tx4 interrupts unconditionally because whenever

Algorithm:  if (room in Tx ring buffer)
              put char in Tx ring buffer
              wrap Tx ring buffer's input ptr if req'd
              update Tx buffer count
              return c
            else
              return EOF

Notes:     

Returns:    c if there was room in buffer.
            EOF if buffer was full.

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_TX4)
int sw_uart4_putchar(int c) {

  if (tx4Count < TX4_BUFF_SIZE) {
    tx4Buff[tx4InP++] = c;

    if (tx4InP > TX4_BUFF_SIZE-1) {
        tx4InP = 0;
    }
    
    tx4Count++;
    
    sw_uart4_hook_enable();

    return c;
  }  
  else {
    return EOF_RETURN_CODE;
  } /* else */
  
} /* sw_uart4_putchar() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
sw_uart4_puts()

Writes the string of characters passed to it out tx4.


Algorithm:  while (end-of-string not reached)
              put char in Tx ring buffer
              increment local pointer
        
Notes:      Differs from stdio.h's puts():
              const char * argument (not char *)
              always returns 0
              assumes stdout is tx4

Returns:    0

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_TX4)
int sw_uart4_puts(const char * cP) {

  while (*cP) {
    sw_uart4_putchar((int) *cP);
    cP++;
  }
  
  return 0;

} /* sw_uart4_puts() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
sw_uart4_outchar()

Takes a character from  tx4's transmitter's ring buffer
and sends it out via sw tx4's transmitter..

To be called from within sw tx4's transmitter's interrupt
service routine.

Algorithm:  

Notes:      Slightly different from the hw UART routines,
             in that the outgoing char is obtained from
             the buffer in SW_UART_TX_STATE_START, but
             the buffer is only updated to reflect this
             in SW_UART_TX_STATE_STOP

Returns:    --

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_TX4)
void sw_uart4_outchar(void) {
  
  static int state = SW_UART_TX_STATE_START;
  static int data;


  if (state == SW_UART_TX_STATE_START) {
    sw_uart4_hook_space();
    data = tx4Buff[tx4OutP];
    state++;
  }
  else if (state < SW_UART_TX_STATE_STOP) {
    if (data & 0x01) {
      sw_uart4_hook_mark();
    }
    else {
      sw_uart4_hook_space();
    }
    data >>= 1;
    state++;
  }
  else if (state == SW_UART_TX_STATE_STOP) {
    sw_uart4_hook_mark();
    state++;
  }
  else {
    tx4OutP++;
    if (tx4OutP > TX4_BUFF_SIZE-1) {
      tx4OutP = 0;
    }
    tx4Count--;

    if (tx4Count == 0) {
      sw_uart4_hook_disable();
    }
    state = SW_UART_TX_STATE_START;
  }
  
} /* sw_uart4_outchar() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
sw_uart4_inchar()

Takes a character from  rx4's receiver
and puts it into rx4's receiver ring buffer

To be called from within rx4's receiver's interrupt
service routine.

Algorithm:  if (room in Rx ring buffer)
             take char from UART rcvr and place in Rx
             ring buffer
            wrap Rx ring buffer's input pointer if req'd
            increment Rx ring buffer count

Notes:      No error checking on incoming chars.

Returns:    --

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_RX4)
void sw_uart4_inchar(unsigned char rxreg) {
  
  if (rx4Count < RX4_BUFF_SIZE) {
    rx4Buff[rx4InP++] = rxreg;
  
    if (rx4InP > RX4_BUFF_SIZE-1) {
        rx4InP = 0;
    }
  
    rx4Count++;  
  }

} /* sw_uart4_inchar() */
#endif
