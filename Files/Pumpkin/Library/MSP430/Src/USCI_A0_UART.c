/******************************************************************************
(C) Copyright Pumpkin, Inc. All Rights Reserved.

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\Library\\MSP430\\Src\\USCI_A0_UART.c,v $
$Author: aek $
$Revision: 3.5 $
$Date: 2009-11-01 22:53:52-08 $

******************************************************************************/

/******************************************************************************

IMPORTANT NOTE: When used on TI's MSP430 -- a 16-bit CPU --
  no protection is required in mainline/task-level/background
  code when manipulating the ring buffers used in these
  routines ** as long as the counts are declared as int
  (i.e. 16 bits) or smaller **. Otherwise they must be
  protected from corruption by ISRs.

******************************************************************************/
#include "config.h"
#include "IO.h"
#include "usci_a_uart.h"



/* control vars and ring buffers for Rx0.   */
#if (ENABLE_RX0)
unsigned int  rx0Count;
unsigned int  rx0InP, rx0OutP;
unsigned char rx0Buff[RX0_BUFF_SIZE];
#endif
    
/* control vars and ring buffers for Tx0.   */
#if (ENABLE_TX0)
unsigned int  tx0Count;
unsigned int  tx0InP, tx0OutP;
unsigned char tx0Buff[TX0_BUFF_SIZE];
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
usci_a0_uart_open()

Conventional initialization, as per TI's recommendations.

We'll be using Tx0 and Rx0 in interrupt-driven mode. Tx0
interrupts are enabled only when there are chars in Tx0's
ring buffer. Rx0 interrupts are always enabled.

Algorithm:  configure Rx & Tx for UART
            clear ring buffers
            initialize UART control, baud rate and modulation
             registers with SWRST as per TI's recommendations
            "release" configuration via SWRST and thereby
             enable USCI for operation
            enable Rx interrupt
      
Notes:    --

Returns:  --      

**                                                                           **
****                                                                       ****
******************************************************************************/
void usci_a0_uart_open(unsigned int CTL0_CTL1,
                       unsigned int MCTL,
                       unsigned int BR1_BR0) {
                       
  usci_a0_uart_open_io();

  #if (ENABLE_RX0)
  rx0Count  =                         0;
  rx0InP    =                         0;
  rx0OutP   =                         0;
  #endif
  #if (ENABLE_TX0)
  tx0Count  =                         0;
  tx0InP    =                         0;
  tx0OutP   =                         0;
  #endif

  UCA0CTL1 |=                   UCSWRST;
  UCA0CTL0  = ((CTL0_CTL1 & 0xFF00)>>8);
  UCA0CTL1  = ((CTL0_CTL1 & 0x00FF)>>0);
  UCA0MCTL  = ((MCTL      & 0x00FF)>>0);
  UCA0BR1   = ((BR1_BR0   & 0xFF00)>>8);
  UCA0BR0   = ((BR1_BR0   & 0x00FF)>>0);
  
  UCA0CTL1 &=                  ~UCSWRST;

  IE2      |=                  UCA0RXIE;
  
} /* usci_a0_uart_open() */


/******************************************************************************
****                                                                       ****
**                                                                           **
usci_a0_uart_close()

Conventional initialization, as per TI's recommendations.

Algorithm:  disable UART's transmitter and receiver
            disable Tx and Rx interrupts
            restore Rx to GPIO (inut)
            restore Tx to GPIO (output, high)
      
Notes:    Does not check buffers for their current status

Returns:  --      

**                                                                           **
****                                                                       ****
******************************************************************************/
void usci_a0_uart_close(void) {

  UCA0CTL1 |=                UCSWRST;

  IE2      &= ~(UCA0TXIE + UCA0RXIE);
  
  usci_a0_uart_close_io();

} /* usci_a0_uart_close() */


/******************************************************************************
****                                                                       ****
**                                                                           **
usci_a0_uart_peek_rx()

Peek into Rx0 buffer at specified location.

Returns:  character at specified location
          EOF if specified location exceeds the number
            of characters in the buffer.      

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_RX0)
int usci_a0_uart_peek_rx(unsigned int index) {

  int c;

  
  if (index >= rx0Count) {
    return EOF_RETURN_CODE;
  }  
  else {  
    if ((rx0OutP+index) > RX0_BUFF_SIZE-1) {
      c = rx0Buff[rx0OutP+index-RX0_BUFF_SIZE];
    }
    
    else {
      c = rx0Buff[rx0OutP+index];
    }
  }
        
  return c;

} /* usci_a0_uart_peek_rx() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
usci_a0_uart_rx_count()

Returns:  Number of characters currently in rx0 buffer.      

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_RX0)
unsigned int usci_a0_uart_rx_count(void) {

  return rx0Count;

} /* usci_a0_uart_rx_count() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
usci_a0_uart_tx_free()

Returns:  Maximimum number of characters that can currently
              be placed into the tx0 buffer.      

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_TX0)
unsigned int usci_a0_uart_tx_free(void) {

  return TX0_BUFF_SIZE-tx0Count;

} /* usci_a0_uart_tx_free() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
usci_a0_uart_getchar()

Removes a character (if present) from the hardware UART's
receiver ring buffer returns it as an int.

Algorithm:  if (char in Rx ring buffer)
              get character from Rx ring buffer
              wrap Rx ring buffer's output ptr if req'd
              decrement Rx ring buffer count
              return character
            else
              return 0  

Notes:     If/when changes to rx0count are not atomic,
             protection is required. As long as it's a
             (16-bit) int, no protection is required.

Returns:    character if a character was present.
            0 if buffer was empty.

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_RX0)
int usci_a0_uart_getchar(void) {

  int c;


  if (rx0Count) {
    c = rx0Buff[rx0OutP++];

    if (rx0OutP > RX0_BUFF_SIZE-1) {
        rx0OutP = 0;
    }
    
    rx0Count--;

    return c;
  }  
  else {
    return EOF_RETURN_CODE;
  }

} /* usci_a0_uart_getchar() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
usci_a0_uart_putchar()

Puts the character into UART0's transmitter's ring buffer
if room is available.

We enable Tx0 interrupts unconditionally because whenever
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

Notes:     If/when changes to tx0count are not atomic,
             protection is required. As long as it's a
             (16-bit) int, no protection is required.

Returns:    c if there was room in buffer.
            EOF if buffer was full.

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_TX0)
int usci_a0_uart_putchar(int c) {

  if (tx0Count < TX0_BUFF_SIZE) {
    tx0Buff[tx0InP++] = c;

    if (tx0InP > TX0_BUFF_SIZE-1) {
        tx0InP = 0;
    }
    
    tx0Count++;
    
    IE2 |= UCA0TXIE;

    return c;
  }  
  else {
    return EOF_RETURN_CODE;
  }

} /* usci_a0_uart_putchar() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
usci_a0_uart_puts()

Writes the string of characters passed to it out Tx0.

We enable Tx0 interrupts unconditionally because whenever
we put a character into the ring buffer, we want the ISR
to occur. Since the interrupt flag is initially set, enabling
interrupts causes us to vector to the ISR, and send the char.

Algorithm:  while (end-of-string not reached)
              put char in Tx ring buffer
              increment local pointer
        
Notes:      Differs from stdio.h's puts():
              const char * argument (not char *)
              always returns 0
              assumes stdout is Tx0

Returns:    0

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_TX0)
int usci_a0_uart_puts(const char * cP) {

  while (*cP) {
    usci_a0_uart_putchar((int) *cP);
    cP++;
  }
  
  return 0;

} /* usci_a0_uart_puts() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
usci_a0_uart_outchar()

Takes a character from  UART0's transmitter's ring buffer
and sends it out via UART0's transmitter..

To be called from within UART0's transmitter's interrupt
service routine (i.e. the ISR at UART0TX_VECTOR).

Algorithm:  take char from Tx ring buffer and place into
             UART's TXBUF
            wrap Tx ring buffer's output pointer if required
            update Tx buffer count
            if (Tx buffer empty)
             disable transmitter interrupts

Notes:    One could argue that one should check if for
           non-zero <tx1Count> before proceeding ... however,
          the only reason we should arrive at the
          UART1TX_VECTOR ISR is because the transmitter
           is ready for another character, AND we just
          enabled its interrupts after adding a character
          to the ring buffer, so a check should never be
          necessary.


Returns:    --

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_TX0)
void usci_a0_uart_outchar(void) {
  
    UCA0TXBUF = tx0Buff[tx0OutP++];

    if (tx0OutP > TX0_BUFF_SIZE-1) {
        tx0OutP = 0;
    }

    tx0Count--;

    if (tx0Count == 0) {
      IE2 &= ~UCA0TXIE;
    }

} /* usci_a0_uart_outchar() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
usci_a0_uart_inchar()

Takes a character from  UART0's receiver
and puts it into UART0's receiver ring buffer

To be called from within UART0's receiver's interrupt
service routine (i.e. the ISR at UART0RX_VECTOR).

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
#if (ENABLE_RX0)
void usci_a0_uart_inchar(unsigned char rxreg) {
  
  if (rx0Count < RX0_BUFF_SIZE) {
    rx0Buff[rx0InP++] = rxreg;
  
    if (rx0InP > RX0_BUFF_SIZE-1) {
        rx0InP = 0;
    }
  
    rx0Count++;  
  }

} /* usci_a0_uart_inchar() */
#endif

