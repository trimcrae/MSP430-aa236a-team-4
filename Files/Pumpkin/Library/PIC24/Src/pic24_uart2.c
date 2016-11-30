/******************************************************************************
(C) Copyright Pumpkin, Inc. All Rights Reserved.

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\Library\\PIC24\\Src\\pic24_uart2.c,v $
$Author: aek $
$Revision: 3.4 $
$Date: 2014-06-02 08:59:58-07 $

******************************************************************************/

/******************************************************************************

IMPORTANT NOTE: MPLAB C30 _cannot_ do something like rx1Count -- atomically. 
Therefore protection of these shared variables is required. In this code, 
the most effective way to do this (that also has the least impact on the rest
of an application using this code) is to explicitly disable, then re-enable
the affected peripheral. A NOP is required to assure that interrupts are in 
fact disabled prior to modifying the shared variable. A listing of the 
resultant code is shown below, as an example:

263:                   DisableIntU1RX;
 03FB6  A96095     bclr.b 0x0095,#3
264:                   __builtin_nop();
 03FB8  000000     nop
265:                   rx1Count--;
 03FBA  804280     mov.w 0x0850,0x0000
 03FBC  E90000     dec.w 0x0000,0x0000
 03FBE  884280     mov.w 0x0000,0x0850
266:                   EnableIntU1RX;
 03FC0  A86095     bset.b 0x0095,#3
 
The NOP is _critical_.

An alternative approach would be through the use of the compiler's intrinsic
SET_AND_SAVE_CPU_IPL and RESTORE_CPU_IPL macros. It was not chosen here, as it
introduces the need to manage interrupt levels.

******************************************************************************/
#include "config.h"
#include "pic24_uart.h"
#include <uart.h>


/* control vars and ring buffers for rx2.   */
#if (ENABLE_RX2)
unsigned int  rx2Count;
unsigned int  rx2InP, rx2OutP;
unsigned char rx2Buff[RX2_BUFF_SIZE];
#endif
    
/* control vars and ring buffers for tx2.   */
#if (ENABLE_TX2)
unsigned int  tx2Count;
unsigned int  tx2InP, tx2OutP;
unsigned char tx2Buff[TX2_BUFF_SIZE];
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
uart2_open()

Via Microchip PIC24F peripheral library.

We'll be using Tx and Rx in interrupt-driven mode. Tx
interrupts are enabled only when there are chars in Tx's
ring buffer. Rx interrupts are always enabled.

Algorithm:  clear ring buffers
            "Turns off the UART module and then disables the UART transmit 
             and receive interrupts. The Interrupt Flag bits are also cleared."
            "Enables/disables the UART transmit and receive interrupts 
			 and sets the interrupt priorities."
		    "configures the UART transmit and receive sections and sets 
			 the communication baud rate."
      
Notes:    --

Returns:  --      

**                                                                           **
****                                                                       ****
******************************************************************************/
void uart2_open(unsigned int config_int,
                unsigned int open1,
                unsigned int open2,
                unsigned int open3) {
                     
  #if (ENABLE_RX2)
  rx2Count  =                                   0;
  rx2InP    =                                   0;
  rx2OutP   =                                   0;
  #endif
  #if (ENABLE_TX2)
  tx2Count  =                                   0;
  tx2InP    =                                   0;
  tx2OutP   =                                   0;
  #endif

  CloseUART2();
  ConfigIntUART2(config_int);
  OpenUART2(open1, open2, open3);

} /* uart2_open() */


/******************************************************************************
****                                                                       ****
**                                                                           **
uart2_close()

Via Microchip PIC24F peripheral library.

Algorithm:  "This function first turns off the UART module and then disables 
            the UART transmit and receive interrupts. The Interrupt Flag bits 
			are also cleared."
      
Notes:    Does not check buffers for their current status

Returns:  --      

**                                                                           **
****                                                                       ****
******************************************************************************/
void uart2_close(void) {

  CloseUART2();

} /* uart2_close() */


/******************************************************************************
****                                                                       ****
**                                                                           **
uart2_peek_rx()

Peek into rx2 buffer at specified location.

Returns:  character at specified location
          EOF if specified location exceeds the number
           of characters in the buffer.      

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_RX2)
int uart2_peek_rx(unsigned int index) {

  int c;
  
  if (index >= rx2Count) {
    return EOF_RETURN_CODE;
  }  
  else {  
    if ((rx2OutP+index) > RX2_BUFF_SIZE-1) {
      c = rx2Buff[rx2OutP+index-RX2_BUFF_SIZE];
    }
    
    else {
      c = rx2Buff[rx2OutP+index];
    }
  } /* else */
        
  return c;

} /* uart2_peek_rx() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
uart2_rx_count()

Returns:  Number of characters currently in rx2 buffer.      

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_RX2)
unsigned int uart2_rx_count(void) {

  return rx2Count;
  
} /* uart2_rx_count() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
uart2_tx_free()

Returns:  Maximimum number of characters that can currently
           be placed into the tx2 buffer.      

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_TX2)
unsigned int uart2_tx_free(void) {

  return TX2_BUFF_SIZE-tx2Count;
  
} /* uart2_tx_free() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
uart2_getchar()

Removes a character (if present) from the hardware UART's
receiver ring buffer returns it as an int.

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
#if (ENABLE_RX2)
int uart2_getchar(void) {

  int c;

  if (rx2Count) {
    c = rx2Buff[rx2OutP++];

    if (rx2OutP > RX2_BUFF_SIZE-1) {
        rx2OutP = 0;
    }

    DisableIntU2RX;
    __builtin_nop();     
    rx2Count--;
    EnableIntU2RX;
    
    return c;
  }
  else {
    return EOF_RETURN_CODE;
  }
  
} /* uart2_getchar() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
uart2_get_byte()

Removes any character (if present, including zero) from the hardware UART's
receiver ring buffer returns it as an unsigned char.

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
#if (ENABLE_RX1)
unsigned char uart2_get_byte(unsigned char * data) {

  if (rx2Count) {
    *data = rx2Buff[rx2OutP++];

    if (rx2OutP > RX2_BUFF_SIZE-1) {
      rx2OutP = 0;
    }
    
    DisableIntU2RX;
    __builtin_nop();
    rx2Count--;
    EnableIntU2RX;

    return 0;
  }
  else {
    return 1; // error
  }
  
} /* uart2_getchar() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
uart2_putchar()

Puts the character into UART2's transmitter's ring buffer
if room is available.

We enable tx2 interrupts unconditionally because whenever
we put a character into the ring buffer, we want the ISR
to occur. Since the interrupt flag is initially set, enabling
interrupts causes us to vector to the ISR, and send the char.

Algorithm:  if (room in Tx ring buffer)
              put char in Tx ring buffer
              wrap Tx ring buffer's input ptr if req'd
              update Tx buffer count
              enable Tx interrupts
              return c
            else
              return EOF

Notes:     

Returns:    c if there was room in buffer.
            EOF if buffer was full.

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_TX2)
int uart2_putchar(int c) {

  if (tx2Count < TX2_BUFF_SIZE) {
    tx2Buff[tx2InP++] = c;

    if (tx2InP > TX2_BUFF_SIZE-1) {
        tx2InP = 0;
    }
 
    DisableIntU2TX;
    __builtin_nop();    
    tx2Count++;
    EnableIntU2TX;

    return c;
  }  
  else {
    return EOF_RETURN_CODE;
  }

} /* uart2_putchar() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
uart2_puts()

Writes the string of characters passed to it out tx2.

We enable tx2 interrupts unconditionally because whenever
we put a character into the ring buffer, we want the ISR
to occur. Since the interrupt flag is initially set, enabling
interrupts causes us to vector to the ISR, and send the char.

Algorithm:  while (end-of-string not reached)
              put char in Tx ring buffer
              increment local pointer
        
Notes:      Differs from stdio.h's puts():
              const char * argument (not char *)
              always returns 0
              assumes stdout is tx2

Returns:    0

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_TX2)
int uart2_puts(const char * cP) {

  while (*cP) {
    uart2_putchar((int) *cP);
    cP++;
  }
  
  return 0;

} /* uart2_puts() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
uart2_outchar()

Takes a character from  UART0's transmitter's ring buffer
and sends it out via UART0's transmitter..

To be called from within UART0's transmitter's interrupt
service routine (i.e. the ISR at _U1TXInterrupt).

Algorithm:  take char from Tx ring buffer and place into
             UART's TXBUF
            wrap Tx ring buffer's output pointer if required
            update Tx buffer count
            if (Tx buffer empty)
             disable transmitter interrupts

Notes:    One could argue that one should check if for
           non-zero <tx2Count> before proceeding ... however,
          the only reason we should arrive at the
          UART2 Tx ISR is because the transmitter
           is ready for another character, AND we just
          enabled its interrupts after adding a character
          to the ring buffer, so a check should never be
          necessary.


Returns:    --

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_TX2)
void uart2_outchar(void) {

    U2TX_Clear_Intr_Status_Bit;

	while(BusyUART2());
    WriteUART2((unsigned int)tx2Buff[tx2OutP++]);

    if (tx2OutP > TX2_BUFF_SIZE-1) {
      tx2OutP = 0;
    }

    tx2Count--;

    if (tx2Count == 0) {
      DisableIntU2TX;
    }

} /* uart2_outchar() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
uart2_inchar()

Takes a character from  UART2's receiver
and puts it into UART2's receiver ring buffer

To be called from within UART2's receiver's interrupt
service routine (i.e. the ISR at _U2RXInterrupt).

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
#if (ENABLE_RX2)
void uart2_inchar(unsigned char rxreg) {

  U2RX_Clear_Intr_Status_Bit;

  if (rx2Count < RX2_BUFF_SIZE) {
    rx2Buff[rx2InP++] = rxreg;
  
    if (rx2InP > RX2_BUFF_SIZE-1) {
        rx2InP = 0;
    }
  
    rx2Count++;  
  }

} /* uart2_inchar() */
#endif
