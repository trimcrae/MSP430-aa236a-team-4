/************************************************************
Copyright (C) 1995-2007 Pumpkin, Inc. and its
Licensor(s).

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\Library\\PIC24\\Inc\\pic24_uart.h,v $
$Author: aek $
$Revision: 3.6 $
$Date: 2014-06-02 09:02:54-07 $

************************************************************/
#ifndef __pic24_uart_h
#define __pic24_uart_h

#include "config.h"

// getchar() and putchar() can be configured to return EOF
//  (default) or 0 on error.
#ifndef GETCHAR_PUTCHAR_RETURN_ZERO
#define  EOF_RETURN_CODE            EOF
#else
#define  EOF_RETURN_CODE            0
#endif


// Default rcv and xmt buffer sizes
#ifndef RX1_BUFF_SIZE
#define RX1_BUFF_SIZE               64
#endif

#ifndef TX1_BUFF_SIZE
#define TX1_BUFF_SIZE               64
#endif

#ifndef RX2_BUFF_SIZE
#define RX2_BUFF_SIZE               64
#endif

#ifndef TX2_BUFF_SIZE
#define TX2_BUFF_SIZE               64
#endif

#ifndef RX3_BUFF_SIZE
#define RX3_BUFF_SIZE               64
#endif

#ifndef TX3_BUFF_SIZE
#define TX3_BUFF_SIZE               64
#endif

#ifndef RX4_BUFF_SIZE
#define RX4_BUFF_SIZE               64
#endif

#ifndef TX4_BUFF_SIZE
#define TX4_BUFF_SIZE               64
#endif

// NOTE that setting either of these buffer sizes to zero DOES NOT DISABLE THE INIT
//  code in open() and close() -- the Tx and Rx sides are still configured for
//  operation. But the buffers, control vars, and individual functions are all
//  disabled when SIZE is set to zero.
#if (RX1_BUFF_SIZE > 0)
#define ENABLE_RX1                      1
#else
#define ENABLE_RX1                      0
#endif
#if (TX1_BUFF_SIZE > 0)
#define ENABLE_TX1                      1
#else
#define ENABLE_TX1                      0
#endif

#if (RX2_BUFF_SIZE > 0)
#define ENABLE_RX2                      1
#else
#define ENABLE_RX2                      0
#endif
#if (TX2_BUFF_SIZE > 0)
#define ENABLE_TX2                      1
#else
#define ENABLE_TX2                      0
#endif

#if (RX3_BUFF_SIZE > 0)
#define ENABLE_RX3                      1
#else
#define ENABLE_RX3                      0
#endif
#if (TX3_BUFF_SIZE > 0)
#define ENABLE_TX3                      1
#else
#define ENABLE_TX3                      0
#endif

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


// UART configuration using AND'ed bitfields, with the following parameters: 
//  ConfigIntUARTx (config)
//  OpenUARTx (config1)
//  OpenUARTx (config2)
//  OpenUARTx (ubrg)
// 9600 baud using low speed
#define UART_9600_N81_MAIN  UART_RX_INT_EN & UART_RX_INT_PR7 & UART_TX_INT_DIS & UART_TX_INT_PR7, \
                            UART_EN & UART_IDLE_CON & UART_IrDA_DISABLE & UART_MODE_FLOW & UART_UEN_00 & UART_DIS_WAKE & \
                              UART_DIS_LOOPBACK & UART_DIS_ABAUD & UART_UXRX_IDLE_ONE & UART_BRGH_SIXTEEN & UART_NO_PAR_8BIT & \
                              UART_1STOPBIT, \
                            UART_INT_TX & UART_IrDA_POL_INV_ZERO & UART_SYNC_BREAK_DISABLED & UART_TX_ENABLE & \
                              UART_INT_RX_CHAR & UART_INT_RX_CHAR & UART_ADR_DETECT_DIS & UART_RX_OVERRUN_CLEAR, \
                            ((MAIN_XTAL_FREQ/(32*9600L))-1)

// 38400 baud using high speed
#define UART_38400_N81_MAIN  UART_RX_INT_EN & UART_RX_INT_PR7 & UART_TX_INT_DIS & UART_TX_INT_PR7, \
                            UART_EN & UART_IDLE_CON & UART_IrDA_DISABLE & UART_MODE_FLOW & UART_UEN_00 & UART_DIS_WAKE & \
                              UART_DIS_LOOPBACK & UART_DIS_ABAUD & UART_UXRX_IDLE_ONE & UART_BRGH_FOUR & UART_NO_PAR_8BIT & \
                              UART_1STOPBIT, \
                            UART_INT_TX & UART_IrDA_POL_INV_ZERO & UART_SYNC_BREAK_DISABLED & UART_TX_ENABLE & \
                              UART_INT_RX_CHAR & UART_INT_RX_CHAR & UART_ADR_DETECT_DIS & UART_RX_OVERRUN_CLEAR, \
                            (((MAIN_XTAL_FREQ/38400L)/8)-1)


// 115200 using high speed                            
#define UART_115200_N81_MAIN  UART_RX_INT_EN & UART_RX_INT_PR7 & UART_TX_INT_DIS & UART_TX_INT_PR7, \
                            UART_EN & UART_IDLE_CON & UART_IrDA_DISABLE & UART_MODE_FLOW & UART_UEN_00 & UART_DIS_WAKE & \
                              UART_DIS_LOOPBACK & UART_DIS_ABAUD & UART_UXRX_IDLE_ONE & UART_BRGH_FOUR & UART_NO_PAR_8BIT & \
                              UART_1STOPBIT, \
                            UART_INT_TX & UART_IrDA_POL_INV_ZERO & UART_SYNC_BREAK_DISABLED & UART_TX_ENABLE & \
                              UART_INT_RX_CHAR & UART_INT_RX_CHAR & UART_ADR_DETECT_DIS & UART_RX_OVERRUN_CLEAR, \
                            (((MAIN_XTAL_FREQ/115200L)/8)-1)

// Function prototypes.
extern void          uart1_close(void);
extern unsigned char uart1_get_byte(unsigned char *);
extern int           uart1_getchar(void);
extern void          uart1_inchar(unsigned char);
extern void          uart1_open(unsigned int, unsigned int, unsigned int, unsigned int);
extern void          uart1_outchar(void);
extern int           uart1_peek_rx(unsigned int);
extern int           uart1_putchar(int);
extern int           uart1_puts(const char *);
extern unsigned int  uart1_rx_count(void);
extern unsigned int  uart1_tx_free(void);

extern void          uart2_close(void);
extern unsigned char uart2_get_byte(unsigned char *); 
extern int           uart2_getchar(void);
extern void          uart2_inchar(unsigned char);
extern void          uart2_open(unsigned int, unsigned int, unsigned int, unsigned int);
extern void          uart2_outchar(void);
extern int           uart2_peek_rx(unsigned int);
extern int           uart2_putchar(int);
extern int           uart2_puts(const char *);
extern unsigned int  uart2_rx_count(void);
extern unsigned int  uart2_tx_free(void);

extern void          uart3_close(void);
extern unsigned char uart3_get_byte(unsigned char *); 
extern int           uart3_getchar(void);
extern void          uart3_inchar(unsigned char);
extern void          uart3_open(unsigned int, unsigned int, unsigned int, unsigned int);
extern void          uart3_outchar(void);
extern int           uart3_peek_rx(unsigned int);
extern int           uart3_putchar(int);
extern int           uart3_puts(const char *);
extern unsigned int  uart3_rx_count(void);
extern unsigned int  uart3_tx_free(void);

extern void          uart4_close(void);
extern unsigned char uart4_get_byte(unsigned char *); 
extern int           uart4_getchar(void);
extern void          uart4_inchar(unsigned char);
extern void          uart4_open(unsigned int, unsigned int, unsigned int, unsigned int);
extern void          uart4_outchar(void);
extern int           uart4_peek_rx(unsigned int);
extern int           uart4_putchar(int);
extern int           uart4_puts(const char *);
extern unsigned int  uart4_rx_count(void);
extern unsigned int  uart4_tx_free(void);

#endif /* __pic24_uart_h */


