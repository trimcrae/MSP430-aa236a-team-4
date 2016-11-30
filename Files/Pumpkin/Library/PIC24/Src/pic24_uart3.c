/******************************************************************************
(C) Copyright Pumpkin, Inc. All Rights Reserved.

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\Library\\PIC24\\Src\\pic24_uart3.c,v $
$Author: aek $
$Revision: 3.0 $
$Date: 2014-03-17 17:50:22-08 $

******************************************************************************/

/******************************************************************************

IMPORTANT NOTE: MPLAB C30 _cannot_ do something like rx3Count -- atomically. 
Therefore protection of these shared variables is required. In this code, 
the most effective way to do this (that also has the least impact on the rest
of an application using this code) is to explicitly disable, then re-enable
the affected peripheral. A NOP is required to assure that interrupts are in 
fact disabled prior to modifying the shared variable. A listing of the 
resultant code is shown below, as an example:

263:                   DisableIntU3RX;
 03FB6  A96095     bclr.b 0x0095,#3
264:                   __builtin_nop();
 03FB8  000000     nop
265:                   rx3Count--;
 03FBA  804280     mov.w 0x0850,0x0000
 03FBC  E90000     dec.w 0x0000,0x0000
 03FBE  884280     mov.w 0x0000,0x0850
266:                   EnableIntU3RX;
 03FC0  A86095     bset.b 0x0095,#3
 
The NOP is _critical_.

An alternative approach would be through the use of the compiler's intrinsic
SET_AND_SAVE_CPU_IPL and RESTORE_CPU_IPL macros. It was not chosen here, as it
introduces the need to manage interrupt levels.

******************************************************************************/
#include "config.h"
#include "pic24_uart.h"
#include <uart.h>


/* control vars and ring buffers for rx3.   */
#if (ENABLE_RX3)
unsigned int  rx3Count;
unsigned int  rx3InP, rx3OutP;
unsigned char rx3Buff[RX3_BUFF_SIZE];
#endif
    
/* control vars and ring buffers for tx3.   */
#if (ENABLE_TX3)
unsigned int  tx3Count;
unsigned int  tx3InP, tx3OutP;
unsigned char tx3Buff[TX3_BUFF_SIZE];
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
uart3_open()

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
void uart3_open(unsigned int config_int,
                unsigned int open1,
                unsigned int open2,
                unsigned int open3) {
                     
  #if (ENABLE_RX3)
  rx3Count  =                                   0;
  rx3InP    =                                   0;
  rx3OutP   =                                   0;
  #endif
  #if (ENABLE_TX3)
  tx3Count  =                                   0;
  tx3InP    =                                   0;
  tx3OutP   =                                   0;
  #endif

  CloseUART3();
  ConfigIntUART3(config_int);
  OpenUART3(open1, open2, open3);

} /* uart3_open() */


/******************************************************************************
****                                                                       ****
**                                                                           **
uart3_close()

Via Microchip PIC24F peripheral library.

Algorithm:  "This function first turns off the UART module and then disables 
            the UART transmit and receive interrupts. The Interrupt Flag bits 
      are also cleared."
      
Notes:    Does not check buffers for their current status

Returns:  --      

**                                                                           **
****                                                                       ****
******************************************************************************/
void uart3_close(void) {

  CloseUART3();

} /* uart3_close() */


/******************************************************************************
****                                                                       ****
**                                                                           **
uart3_peek_rx()

Peek into rx3 buffer at specified location.

Returns:  character at specified location
          EOF if specified location exceeds the number
           of characters in the buffer.      

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_RX3)
int uart3_peek_rx(unsigned int index) {

  int c;

  
  if (index >= rx3Count) {
    return EOF_RETURN_CODE;
  }  
  else {  
    if ((rx3OutP+index) > RX3_BUFF_SIZE-1) {
      c = rx3Buff[rx3OutP+index-RX3_BUFF_SIZE];
    }
    
    else {
      c = rx3Buff[rx3OutP+index];
    }
  } /* else */
        
  return c;

} /* uart3_peek_rx() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
uart3_rx_count()

Returns:  Number of characters currently in rx3 buffer.      

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_RX3)
unsigned int uart3_rx_count(void) {

  return rx3Count;
  
} /* uart3_rx_count() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
uart3_tx_free()

Returns:  Maximimum number of characters that can currently
           be placed into the tx3 buffer.      

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_TX3)
unsigned int uart3_tx_free(void) {

  return TX3_BUFF_SIZE-tx3Count;
  
} /* uart3_tx_free() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
uart3_getchar()

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
#if (ENABLE_RX3)
int uart3_getchar(void) {

  int c;


  if (rx3Count) {
    c = rx3Buff[rx3OutP++];

    if (rx3OutP > RX3_BUFF_SIZE-1) {
        rx3OutP = 0;
    }
    
    DisableIntU3RX;
    __builtin_nop();    
    rx3Count--;
    EnableIntU3RX;

    return c;
  }
  else {
    return EOF_RETURN_CODE;
  }
  
} /* uart3_getchar() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
uart3_get_byte()

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
#if (ENABLE_RX3)
unsigned char uart3_get_byte(unsigned char * data) {

  if (rx3Count) {
    *data = rx3Buff[rx3OutP++];

    if (rx3OutP > RX3_BUFF_SIZE-1) {
      rx3OutP = 0;
    }
    
    DisableIntU3RX;
    __builtin_nop();
    rx3Count--;
    EnableIntU3RX;

    return 0;
  }
  else {
    return 1; // error
  }
  
} /* uart3_getchar() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
uart3_putchar()

Puts the character into UART3's transmitter's ring buffer
if room is available.

We enable tx3 interrupts unconditionally because whenever
we put a character into the ring buffer, we want the ISR
to occur. Since the interrupt flag is initially set, enabling
interrupts causes us to vector to the ISR, and send the char.

Algorithm:  if (room in Tx ring buffer)
              put char in Tx ring buffer
              wrap Tx ring buffer's input ptr if req'd
              update Tx buffer count
              enable Tx interrupts (unconditionally)
              return c
            else
              return EOF

Notes:     

Returns:    c if there was room in buffer.
            EOF if buffer was full.

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_TX3)
int uart3_putchar(int c) {

  if (tx3Count < TX3_BUFF_SIZE) {
    tx3Buff[tx3InP++] = c;

    if (tx3InP > TX3_BUFF_SIZE-1) {
        tx3InP = 0;
    }
    
    DisableIntU3TX;
    __builtin_nop();    
    tx3Count++;
    EnableIntU3TX;

    return c;
  }  
  else {
    return EOF_RETURN_CODE;
  }

} /* uart3_putchar() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
uart3_puts()

Writes the string of characters passed to it out tx3.

We enable tx3 interrupts unconditionally because whenever
we put a character into the ring buffer, we want the ISR
to occur. Since the interrupt flag is initially set, enabling
interrupts causes us to vector to the ISR, and send the char.

Algorithm:  while (end-of-string not reached)
              put char in Tx ring buffer
              increment local pointer
        
Notes:      Differs from stdio.h's puts():
              const char * argument (not char *)
              always returns 0
              assumes stdout is tx3

Returns:    0

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_TX3)
int uart3_puts(const char * cP) {

  while (*cP) {
    uart3_putchar((int) *cP);
    cP++;
  }
  
  return 0;

} /* uart3_puts() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
uart3_outchar()

Takes a character from  UART0's transmitter's ring buffer
and sends it out via UART0's transmitter..

To be called from within UART0's transmitter's interrupt
service routine (i.e. the ISR at _U3TXInterrupt).

Algorithm:  take char from Tx ring buffer and place into
             UART's TXBUF
            wrap Tx ring buffer's output pointer if required
            update Tx buffer count
            if (Tx buffer empty)
             disable transmitter interrupts

Notes:    One could argue that one should check if for
           non-zero <tx3Count> before proceeding ... however,
          the only reason we should arrive at the
          UART3 Tx ISR is because the transmitter
           is ready for another character, AND we just
          enabled its interrupts after adding a character
          to the ring buffer, so a check should never be
          necessary.


Returns:    --

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_TX3)
void uart3_outchar(void) {

  U3TX_Clear_Intr_Status_Bit;

  while(BusyUART3());
  WriteUART3((unsigned int)tx3Buff[tx3OutP++]);

  if (tx3OutP > TX3_BUFF_SIZE-1) {
    tx3OutP = 0;
  }

  tx3Count--;

  if (tx3Count == 0) {
    DisableIntU3TX;
  }

} /* uart3_outchar() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
uart3_inchar()

Takes a character from  UART3's receiver
and puts it into UART3's receiver ring buffer

To be called from within UART3's receiver's interrupt
service routine (i.e. the ISR at _U3RXInterrupt).

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
#if (ENABLE_RX3)
void uart3_inchar(unsigned char rxreg) {

  U3RX_Clear_Intr_Status_Bit;
 
  if (rx3Count < RX3_BUFF_SIZE) {
    rx3Buff[rx3InP++] = rxreg;
  
    if (rx3InP > RX3_BUFF_SIZE-1) {
        rx3InP = 0;
    }
  
    rx3Count++;  
  }

} /* uart3_inchar() */
#endif
