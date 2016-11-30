/************************************************************
Copyright (C) 1995-2007 Pumpkin, Inc. and its
Licensor(s).

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\Library\\MSP430\\Inc\\USART_UART.h,v $
$Author: aek $
$Revision: 3.7 $
$Date: 2009-11-02 00:45:03-08 $

************************************************************/

#ifndef __usart_uart_h
#define __usart_uart_h

#include "config.h"

// For PM packages only ...
#if   defined(__MSP430F149__)  || (__TARGET_PROCESSOR==MSP430F149)  \
   || defined(__MSP430F169__)  || (__TARGET_PROCESSOR==MSP430F169)  \
   || defined(__MSP430F1611__) || (__TARGET_PROCESSOR==MSP430F1611) \
   || defined(__MSP430F1612__) || (__TARGET_PROCESSOR==MSP430F1612)
#define usart_uart0_open_io()      do { IO_LOW(P3,BIT5); \
                                        IO_INPUT(P3,BIT5); \
                                        IO_HIGH(P3,BIT4); \
                                        IO_OUTPUT(P3,BIT4); \
                                        IO_SEL(P3,BIT5+BIT4); } while (0)

#define usart_uart1_open_io()      do { IO_LOW(P3,BIT7); \
                                        IO_INPUT(P3,BIT7); \
                                        IO_HIGH(P3,BIT6); \
                                        IO_OUTPUT(P3,BIT6); \
                                        IO_SEL(P3,BIT7+BIT6); } while (0)

#define usart_uart0_close_io()     do { IO_HIGH(P3,BIT4); \
                                        IO_UNSEL(P3,BIT5+BIT4); } while (0)

#define usart_uart1_close_io()     do { IO_HIGH(P3,BIT6); \
                                        IO_UNSEL(P3,BIT7+BIT6); } while (0)

// For PZ packages only ...
#elif defined(__MSP430FG4616__) || (__TARGET_PROCESSOR==MSP430FG4616) \
   || defined(__MSP430FG4617__) || (__TARGET_PROCESSOR==MSP430FG4617) \
   || defined(__MSP430FG4618__) || (__TARGET_PROCESSOR==MSP430FG4618) \
   || defined(__MSP430FG4619__) || (__TARGET_PROCESSOR==MSP430FG4619)
#define usart_uart1_open_io()      do { IO_LOW(P4,BIT1); \
                                        IO_INPUT(P4,BIT1); \
                                        IO_HIGH(P4,BIT0); \
                                        IO_OUTPUT(P4,BIT0); \
                                        IO_SEL(P4,BIT1+BIT0); } while (0)

#define usart_uart1_close_io()     do { IO_HIGH(P4,BIT1); \
                                        IO_UNSEL(P4,BIT1+BIT0); } while (0)

#else
#error  usart_uart.h requires port pin definitions for UART0 and/or UART1.

#endif  /* defined (__MSP430F167__) || ... */



// Must be defined for UART initialization parameters.
#ifndef ACLK
#error ACLK not defined.
#endif

#ifndef SMCLK
#error SMCLK not defined.
#endif



// getchar() and putchar() can be configured to return EOF
//  (default) or 0 on error.
#ifndef GETCHAR_PUTCHAR_RETURN_ZERO
#define  EOF_RETURN_CODE            EOF
#else
#define  EOF_RETURN_CODE            0
#endif


// Default rcv and xmt buffer sizes
#ifndef RX0_BUFF_SIZE
#define RX0_BUFF_SIZE               64
#endif

#ifndef TX0_BUFF_SIZE
#define TX0_BUFF_SIZE               64
#endif

#ifndef RX1_BUFF_SIZE
#define RX1_BUFF_SIZE               64
#endif

#ifndef TX1_BUFF_SIZE
#define TX1_BUFF_SIZE               64
#endif

// NOTE that setting either of these buffer sizes to zero DOES NOT DISABLE THE INIT
//  code in open() and close() -- the Tx and Rx sides are still configured for
//  operation. But the buffers, control vars, and individual functions are all
//  disabled when SIZE is set to zero.
#if (RX0_BUFF_SIZE > 0)
#define ENABLE_RX0                      1
#else
#define ENABLE_RX0                      0
#endif
#if (TX0_BUFF_SIZE > 0)
#define ENABLE_TX0                      1
#else
#define ENABLE_TX0                      0
#endif

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


// UART rates for 32.768kHz ACLK and 1152.kHz <= SMCLK <= 16MHz.
#define USART_UART_300_N81_ACLK         (       CHAR <<8)+(      SSEL0) , 0x0000, 0x006C // needs modulation
#define USART_UART_600_N81_ACLK         (       CHAR <<8)+(      SSEL0) , 0x0000, 0x0036 // needs modulation
#define USART_UART_1200_N81_ACLK        (       CHAR <<8)+(      SSEL0) , 0x0003, 0x001B
#define USART_UART_2400_N81_ACLK        (       CHAR <<8)+(      SSEL0) , 0x006B, 0x000D
#define USART_UART_4800_N81_ACLK        (       CHAR <<8)+(      SSEL0) , 0x006F, 0x0006
#define USART_UART_9600_N81_ACLK        (       CHAR <<8)+(      SSEL0) , 0x004A, 0x0003
#define USART_UART_300_N81_SMCLK        (       CHAR <<8)+(SSEL1+SSEL0) , 0x0000, (SMCLK/   300)
#define USART_UART_600_N81_SMCLK        (       CHAR <<8)+(SSEL1+SSEL0) , 0x0000, (SMCLK/   600)
#define USART_UART_1200_N81_SMCLK       (       CHAR <<8)+(SSEL1+SSEL0) , 0x0000, (SMCLK/  1200)
#define USART_UART_2400_N81_SMCLK       (       CHAR <<8)+(SSEL1+SSEL0) , 0x0000, (SMCLK/  2400)
#define USART_UART_4800_N81_SMCLK       (       CHAR <<8)+(SSEL1+SSEL0) , 0x0000, (SMCLK/  4800)
#define USART_UART_4800_O81_SMCLK       (((PENA+CHAR)<<8)+(SSEL1+SSEL0)), 0x0000, (SMCLK/  4800)
#define USART_UART_9600_N81_SMCLK       (       CHAR <<8)+(SSEL1+SSEL0) , 0x0000, (SMCLK/  9600)
#define USART_UART_9600_O81_SMCLK       (((PENA+CHAR)<<8)+(SSEL1+SSEL0)), 0x0000, (SMCLK/  9600)
#define USART_UART_19200_N81_SMCLK      (       CHAR <<8)+(SSEL1+SSEL0) , 0x0000, (SMCLK/ 19200)
#define USART_UART_38400_N81_SMCLK      (       CHAR <<8)+(SSEL1+SSEL0) , 0x0000, (SMCLK/ 38400)
#define USART_UART_57600_N81_SMCLK      (       CHAR <<8)+(SSEL1+SSEL0) , 0x0000, (SMCLK/ 57600)
#define USART_UART_76800_N81_SMCLK      (       CHAR <<8)+(SSEL1+SSEL0) , 0x0000, (SMCLK/ 76800)
#define USART_UART_115200_N81_SMCLK     (       CHAR <<8)+(SSEL1+SSEL0) , 0x0000, (SMCLK/115200)


// Function prototypes.
extern void          usart_uart0_close(void);
extern int           usart_uart0_getchar(void);
extern void          usart_uart0_inchar(unsigned char);
extern void          usart_uart0_open(unsigned int, unsigned int, unsigned int);
extern void          usart_uart0_outchar(void);
extern int           usart_uart0_peek_rx(unsigned int);
extern int           usart_uart0_putchar(int);
extern int           usart_uart0_puts(const char *);
extern unsigned int  usart_uart0_rx_count(void);
extern unsigned int  usart_uart0_tx_free(void);

extern void          usart_uart1_close(void);
extern int           usart_uart1_getchar(void);
extern void          usart_uart1_inchar(unsigned char);
extern void          usart_uart1_open(unsigned int, unsigned int, unsigned int);
extern void          usart_uart1_outchar(void);
extern int           usart_uart1_peek_rx(unsigned int);
extern int           usart_uart1_putchar(int);
extern int           usart_uart1_puts(const char *);
extern unsigned int  usart_uart1_rx_count(void);
extern unsigned int  usart_uart1_tx_free(void);

#endif /* __usart_uart_h */


