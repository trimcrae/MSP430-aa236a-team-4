/******************************************************************************
(C) Copyright Pumpkin, Inc. All Rights Reserved.

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\Library\\MSP430\\Src\\USART_UART1.c,v $
$Author: aek $
$Revision: 3.6 $
$Date: 2009-11-01 22:53:51-08 $

******************************************************************************/
#include "config.h"
#include "IO.h"
#include "usart_uart.h"


#if (ENABLE_RX1)
unsigned int  rx1Count;
unsigned int  rx1InP, rx1OutP;
unsigned char rx1Buff[RX1_BUFF_SIZE];
#endif

#if (ENABLE_TX1)
unsigned int  tx1Count;
unsigned int  tx1InP, tx1OutP;
unsigned char tx1Buff[TX1_BUFF_SIZE];
#endif

/******************************************************************************
****                                                                       ****
**                                                                           **
usart_uart1_open()

**                                                                           **
****                                                                       ****
******************************************************************************/
void usart_uart1_open(unsigned int UCTL_UTCTL,
                      unsigned int URCTL_UMCTL,
                      unsigned int BR1_BR0) {
                      
  usart_uart1_open_io();

  #if (ENABLE_RX1)
  rx1Count =                                   0;
  rx1InP   =                                   0;
  rx1OutP  =                                   0;
  #endif
  #if (ENABLE_TX1)
  tx1Count =                                   0;
  tx1InP   =                                   0;
  tx1OutP  =                                   0;
  #endif

  UCTL1    = ((UCTL_UTCTL  & 0xFF00)>>8) + SWRST;
  UTCTL1   = ((UCTL_UTCTL  & 0x00FF)>>0)        ;
  URCTL1   = ((URCTL_UMCTL & 0xFF00)>>8)        ;
  UMCTL1   = ((URCTL_UMCTL & 0x00FF)>>0)        ;
  UBR11    = ((BR1_BR0     & 0xFF00)>>8)        ;
  UBR01    = ((BR1_BR0     & 0x00FF)>>0)        ;

  ME2     |=                       UTXE1 + URXE1;
  
  UCTL1   &=                              ~SWRST;
  
  IE2     |=                              URXIE1;
  
} /* usart_uart1_open() */


/******************************************************************************
****                                                                       ****
**                                                                           **
usart_uart1_close()

**                                                                           **
****                                                                       ****
******************************************************************************/
void usart_uart1_close(void) {

  UCTL1    |=              SWRST;

  ME2      &=   ~(UTXE1 + URXE1);
    
  UCTL1    &=             ~SWRST;

  IE2      &= ~(UTXIE1 + URXIE1);
  
  usart_uart1_close_io();
  
} /* usart_uart1_close() */


/******************************************************************************
****                                                                       ****
**                                                                           **
usart_uart1_peek()

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_RX1)
int usart_uart1_peek(unsigned int index) {

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
  
} /* usart_uart1_peek() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
usart_uart1_rx_count()

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_RX1)
unsigned int usart_uart1_rx_count(void) {

  return rx1Count;

} /* usart_uart1_rx_count() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
usart_uart1_tx_free()

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_TX1)
unsigned int usart_uart1_tx_free(void) {

  return TX1_BUFF_SIZE-tx1Count;
  
} /* usart_uart1_tx_free() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
usart_uart1_getchar()

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_RX1)
int usart_uart1_getchar(void) {

  int c;


  if (rx1Count) {
    c = rx1Buff[rx1OutP++];

    if (rx1OutP > RX1_BUFF_SIZE-1) {
      rx1OutP = 0;
    }
    
    rx1Count--;

    return c;
  }
  else {
    return EOF_RETURN_CODE;
  }
  
} /* usart_uart1_getchar() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
usart_uart1_putchar()

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_TX1)
int usart_uart1_putchar(int c) {

  if (tx1Count < TX1_BUFF_SIZE) {
    tx1Buff[tx1InP++] = c;

    if (tx1InP > TX1_BUFF_SIZE-1) {
      tx1InP = 0;
    }
    
    tx1Count++;
    IE2 |= UTXIE1;

    return c;
  }  
  else {
    return EOF_RETURN_CODE;
  }

} /* usart_uart1_putchar() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
usart_uart1_puts()

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_TX1)
int usart_uart1_puts(const char * cP) {

  while (*cP) {
    usart_uart1_putchar((int) *cP);
    cP++;
  }
  
  return 0;

} /* usart_uart1_puts() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
usart_uart1_outchar()

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_TX1)
void usart_uart1_outchar(void) {

    TXBUF1 = tx1Buff[tx1OutP++];

    if (tx1OutP > TX1_BUFF_SIZE-1) {
        tx1OutP = 0;
    }

    tx1Count--;

    if (tx1Count == 0) {
      IE2 &= ~UTXIE1;
    }

} /* usart_uart1_outchar() */
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
usart_uart1_inchar()

**                                                                           **
****                                                                       ****
******************************************************************************/
#if (ENABLE_RX1) 
void usart_uart1_inchar(unsigned char rxreg) {
  
  if (rx1Count < RX1_BUFF_SIZE) {
    rx1Buff[rx1InP++] = rxreg;
  
    if (rx1InP > RX1_BUFF_SIZE-1) {
      rx1InP = 0;
    }
  
    rx1Count++;
  }

} /* usart_uart1_inchar() */
#endif
