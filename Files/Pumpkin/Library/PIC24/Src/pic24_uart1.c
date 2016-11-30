/******************************************************************************
(C) Copyright Pumpkin, Inc. All Rights Reserved.

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\Library\\PIC24\\Src\\pic24_uart1.c,v $
$Author: aek $
$Revision: 3.4 $
$Date: 2014-06-02 08:59:59-07 $

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
#include "pic24_uart.h"
#include "config.h"
#include <uart.h>


/* control vars and ring buffers for rx1.   */
#if (ENABLE_RX1)
unsigned int  rx1Count;
unsigned int  rx1InP, rx1OutP;
unsigned char rx1Buff[RX1_BUFF_SIZE];
#endif
    
/* control vars and ring buffers for tx1.   */
#if (ENABLE_TX1)
unsigned int  tx1Count;
unsigned int  tx1InP, tx1OutP;
unsigned char tx1Buff[TX1_BUFF_SIZE];
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
uart1_open()

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
void uart1_open(unsigned int config_int,
                unsigned int open1,
                unsigned int open2,
                unsigned int open3) {
                     
  #if (ENABLE_RX1)
  rx1Count  =                                   0;
  rx1InP    =                                   0;
  rx1OutP   =                                   0;
  #endif
  #if (ENABLE_TX1)
  tx1Count  =                                   0;
  tx1InP    =                                   0;
  tx1OutP   =                                   0;
  #endif

  CloseUART1();
  ConfigIntUART1(config_int);
  OpenUART1(open1, open2, open3);

} /* uart1_open() */


/******************************************************************************
****                                                                       ****
**                                                                           **
uart1_close()

Via Microchip PIC24F peripheral library.

Algorithm:  "This function first turns off the UART module and then disables 
            the UART transmit and receive interrupts. The Interrupt Flag bits 
      are also cleared."
      
Notes:    Does not check buffers for their current status

Returns:  --      

**                                                                           **
****                                                                       ****
******************************************************************************/
void uart1_close(void) {

  CloseUART1();

} /* uart1_close() */


/******************************************************************************
****                                                                       ****
**                                                                           **
uart1_peek_rx()

Peek into rx1 buffer at specified location.

Returns:  character at specified location
          EOF if specified location exceeds the number
           of characters in the buffer.      

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_RX1)
int uart1_peek_rx(unsigned int index) {

  int c;

  
  if (index >= rx1Count) {
    return EOF_RETURN_CODE;
  }  
  else {  
    if ((rx1OutP+index) > RX1_BUFF_SIZE-1) {
      c = rx1Buff[rx1OutP+index-RX1_BUFF_SIZE];
    }
    
    else {
      c = rx1Buff[rx1OutP+index];
    }
  } /* else */
        
  return c;

} /* uart1_peek_rx() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
uart1_rx_count()

Returns:  Number of characters currently in rx1 buffer.      

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_RX1)
unsigned int uart1_rx_count(void) {

  return rx1Count;
  
} /* uart1_rx_count() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
uart1_tx_free()

Returns:  Maximimum number of characters that can currently
           be placed into the tx1 buffer.      

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_TX1)
unsigned int uart1_tx_free(void) {

  return TX1_BUFF_SIZE-tx1Count;
  
} /* uart1_tx_free() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
uart1_getchar()

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
#if (ENABLE_RX1)
int uart1_getchar(void) {

  int c;


  if (rx1Count) {
    c = rx1Buff[rx1OutP++];

    if (rx1OutP > RX1_BUFF_SIZE-1) {
        rx1OutP = 0;
    }
    
    DisableIntU1RX;
    __builtin_nop();    
    rx1Count--;
    EnableIntU1RX;

    return c;
  }
  else {
    return EOF_RETURN_CODE;
  }
  
} /* uart1_getchar() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
uart1_get_byte()

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
unsigned char uart1_get_byte(unsigned char * data) {

  if (rx1Count) {
    *data = rx1Buff[rx1OutP++];

    if (rx1OutP > RX1_BUFF_SIZE-1) {
      rx1OutP = 0;
    }
    
    DisableIntU1RX;
    __builtin_nop();
    rx1Count--;
    EnableIntU1RX;

    return 0;
  }
  else {
    return 1; // error
  }
  
} /* uart1_getchar() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
uart1_putchar()

Puts the character into UART1's transmitter's ring buffer
if room is available.

We enable tx1 interrupts unconditionally because whenever
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
#if (ENABLE_TX1)
int uart1_putchar(int c) {

  if (tx1Count < TX1_BUFF_SIZE) {
    tx1Buff[tx1InP++] = c;

    if (tx1InP > TX1_BUFF_SIZE-1) {
        tx1InP = 0;
    }
    
    DisableIntU1TX;
    __builtin_nop();    
    tx1Count++;
    EnableIntU1TX;

    return c;
  }  
  else {
    return EOF_RETURN_CODE;
  }

} /* uart1_putchar() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
uart1_puts()

Writes the string of characters passed to it out tx1.

We enable tx1 interrupts unconditionally because whenever
we put a character into the ring buffer, we want the ISR
to occur. Since the interrupt flag is initially set, enabling
interrupts causes us to vector to the ISR, and send the char.

Algorithm:  while (end-of-string not reached)
              put char in Tx ring buffer
              increment local pointer
        
Notes:      Differs from stdio.h's puts():
              const char * argument (not char *)
              always returns 0
              assumes stdout is tx1

Returns:    0

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_TX1)
int uart1_puts(const char * cP) {

  while (*cP) {
    uart1_putchar((int) *cP);
    cP++;
  }
  
  return 0;

} /* uart1_puts() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
uart1_outchar()

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
           non-zero <tx1Count> before proceeding ... however,
          the only reason we should arrive at the
          UART1 Tx ISR is because the transmitter
           is ready for another character, AND we just
          enabled its interrupts after adding a character
          to the ring buffer, so a check should never be
          necessary.


Returns:    --

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_TX1)
void uart1_outchar(void) {

  U1TX_Clear_Intr_Status_Bit;

  while(BusyUART1());
  WriteUART1((unsigned int)tx1Buff[tx1OutP++]);

  if (tx1OutP > TX1_BUFF_SIZE-1) {
    tx1OutP = 0;
  }

  tx1Count--;

  if (tx1Count == 0) {
    DisableIntU1TX;
  }

} /* uart1_outchar() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
uart1_inchar()

Takes a character from  UART1's receiver
and puts it into UART1's receiver ring buffer

To be called from within UART1's receiver's interrupt
service routine (i.e. the ISR at _U1RXInterrupt).

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
#if (ENABLE_RX1)
void uart1_inchar(unsigned char rxreg) {
	
  U1RX_Clear_Intr_Status_Bit;
 
  if (rx1Count < RX1_BUFF_SIZE) {
    rx1Buff[rx1InP++] = rxreg;
  
    if (rx1InP > RX1_BUFF_SIZE-1) {
        rx1InP = 0;
    }
  
    rx1Count++;  
  }

} /* uart1_inchar() */
#endif
