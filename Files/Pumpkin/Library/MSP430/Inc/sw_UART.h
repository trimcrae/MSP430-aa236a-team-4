/************************************************************
Copyright (C) 1995-2007 Pumpkin, Inc. and its
Licensor(s).

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\Library\\MSP430\\Inc\\sw_UART.h,v $
$Author: aek $
$Revision: 3.2 $
$Date: 2009-11-01 22:53:21-08 $

************************************************************/

#ifndef __sw_uart_h
#define __sw_uart_h

#include "config.h"


// getchar() and putchar() can be configured to return EOF
//  (default) or 0 on error.
#ifndef GETCHAR_PUTCHAR_RETURN_ZERO
#define  EOF_RETURN_CODE            EOF
#else
#define  EOF_RETURN_CODE            0
#endif


// For sw state machines.
#define SW_UART_TX_STATE_START          0
#define SW_UART_TX_STATE_STOP           9

// Default rcv and xmt buffer sizes
#ifndef RX4_BUFF_SIZE
#define RX4_BUFF_SIZE             64
#endif

#ifndef TX4_BUFF_SIZE
#define TX4_BUFF_SIZE             64
#endif

// NOTE that setting either of these buffer sizes to zero DOES NOT DISABLE THE INIT
//  code in open() and close() -- the Tx and Rx sides are still configured for
//  operation. But the buffers, control vars, and individual functions are all
//  disabled when SIZE is set to zero.
#if (RX4_BUFF_SIZE > 0)
#define ENABLE_RX4                      1
#else
#define ENABLE_RX4                      0
#endif
#if (TX4_BUFF_SIZE > 0)
#define ENABLE_TX4                      1
#else
#define ENABLE_TX4                      0
#endif


// Function prototypes.
extern void          sw_uart4_close(void);
extern int           sw_uart4_getchar(void);
extern void          sw_uart4_inchar(unsigned char);
extern void          sw_uart4_open(unsigned int);
extern void          sw_uart4_outchar(void);
extern int           sw_uart4_peek_rx(unsigned int);
extern int           sw_uart4_putchar(int);
extern int           sw_uart4_puts(const char *);
extern unsigned int  sw_uart4_rx_count(void);
extern unsigned int  sw_uart4_tx_free(void);


extern void          sw_uart4_hook_open(void);
extern void          sw_uart4_hook_close(void);
extern void          sw_uart4_hook_enable(void);
extern void          sw_uart4_hook_disable(void);
extern void          sw_uart4_hook_mark(void);
extern void          sw_uart4_hook_space(void);

#endif /* __sw_uart_h */


