/******************************************************************************
(C) Copyright Pumpkin, Inc. All Rights Reserved.

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\Library\\PIC24\\Src\\pic24_uart4.c,v $
$Author: aek $
$Revision: 3.0 $
$Date: 2014-03-17 17:50:21-08 $

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


/* control vars and ring buffers for rx4.   */
#if (ENABLE_RX4)
unsigned int  rx4Count;
unsigned int  rx4InP, rx4OutP;
unsigned char rx4Buff[RX4_BUFF_SIZE];
#endif
    
/* control vars and ring buffers for tx4.   */
#if (ENABLE_TX4)
unsigned int  tx4Count;
unsigned int  tx4InP, tx4OutP;
unsigned char tx4Buff[TX4_BUFF_SIZE];
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
uart4_open()

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
void uart4_open(unsigned int config_int,
                unsigned int open1,
                unsigned int open2,
                unsigned int open3) {
                     
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

  CloseUART4();
  ConfigIntUART4(config_int);
  OpenUART4(open1, open2, open3);

} /* uart4_open() */


/******************************************************************************
****                                                                       ****
**                                                                           **
uart4_close()

Via Microchip PIC24F peripheral library.

Algorithm:  "This function first turns off the UART module and then disables 
            the UART transmit and receive interrupts. The Interrupt Flag bits 
			are also cleared."
      
Notes:    Does not check buffers for their current status

Returns:  --      

**                                                                           **
****                                                                       ****
******************************************************************************/
void uart4_close(void) {

  CloseUART4();

} /* uart4_close() */


/******************************************************************************
****                                                                       ****
**                                                                           **
uart4_peek_rx()

Peek into rx4 buffer at specified location.

Returns:  character at specified location
          EOF if specified location exceeds the number
           of characters in the buffer.      

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_RX4)
int uart4_peek_rx(unsigned int index) {

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

} /* uart4_peek_rx() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
uart4_rx_count()

Returns:  Number of characters currently in rx4 buffer.      

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_RX4)
unsigned int uart4_rx_count(void) {

  return rx4Count;
  
} /* uart4_rx_count() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
uart4_tx_free()

Returns:  Maximimum number of characters that can currently
           be placed into the tx4 buffer.      

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_TX4)
unsigned int uart4_tx_free(void) {

  return TX4_BUFF_SIZE-tx4Count;
  
} /* uart4_tx_free() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
uart4_getchar()

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
#if (ENABLE_RX4)
int uart4_getchar(void) {

  int c;


  if (rx4Count) {
    c = rx4Buff[rx4OutP++];

    if (rx4OutP > RX4_BUFF_SIZE-1) {
        rx4OutP = 0;
    }

    DisableIntU4RX;
    __builtin_nop();     
    rx4Count--;
    EnableIntU4RX;
    

    return c;
  }
  else {
    return EOF_RETURN_CODE;
  }
  
} /* uart4_getchar() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
uart4_get_byte()

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
unsigned char uart4_get_byte(unsigned char * data) {

  if (rx4Count) {
    *data = rx4Buff[rx4OutP++];

    if (rx4OutP > RX4_BUFF_SIZE-1) {
      rx4OutP = 0;
    }
    
    DisableIntU4RX;
    __builtin_nop();
    rx4Count--;
    EnableIntU4RX;

    return 0;
  }
  else {
    return 1; // error
  }
  
} /* uart4_getchar() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
uart4_putchar()

Puts the character into UART4's transmitter's ring buffer
if room is available.

We enable tx4 interrupts unconditionally because whenever
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
#if (ENABLE_TX4)
int uart4_putchar(int c) {

  if (tx4Count < TX4_BUFF_SIZE) {
    tx4Buff[tx4InP++] = c;

    if (tx4InP > TX4_BUFF_SIZE-1) {
        tx4InP = 0;
    }
 
    DisableIntU4TX;
    __builtin_nop();    
    tx4Count++;
    EnableIntU4TX;

    return c;
  }  
  else {
    return EOF_RETURN_CODE;
  }

} /* uart4_putchar() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
uart4_puts()

Writes the string of characters passed to it out tx4.

We enable tx4 interrupts unconditionally because whenever
we put a character into the ring buffer, we want the ISR
to occur. Since the interrupt flag is initially set, enabling
interrupts causes us to vector to the ISR, and send the char.

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
int uart4_puts(const char * cP) {

  while (*cP) {
    uart4_putchar((int) *cP);
    cP++;
  }
  
  return 0;

} /* uart4_puts() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
uart4_outchar()

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
           non-zero <tx4Count> before proceeding ... however,
          the only reason we should arrive at the
          UART4 Tx ISR is because the transmitter
           is ready for another character, AND we just
          enabled its interrupts after adding a character
          to the ring buffer, so a check should never be
          necessary.


Returns:    --

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_TX4)
void uart4_outchar(void) {

    U4TX_Clear_Intr_Status_Bit;

	while(BusyUART4());
    WriteUART4((unsigned int)tx4Buff[tx4OutP++]);

    if (tx4OutP > TX4_BUFF_SIZE-1) {
      tx4OutP = 0;
    }

    tx4Count--;

    if (tx4Count == 0) {
      DisableIntU4TX;
    }

} /* uart4_outchar() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
uart4_inchar()

Takes a character from  UART4's receiver
and puts it into UART4's receiver ring buffer

To be called from within UART4's receiver's interrupt
service routine (i.e. the ISR at _U4RXInterrupt).

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
void uart4_inchar(unsigned char rxreg) {

  U4RX_Clear_Intr_Status_Bit;

  if (rx4Count < RX4_BUFF_SIZE) {
    rx4Buff[rx4InP++] = rxreg;
  
    if (rx4InP > RX4_BUFF_SIZE-1) {
        rx4InP = 0;
    }
  
    rx4Count++;  
  }

} /* uart4_inchar() */
#endif
