/******************************************************************************
(C) Copyright Pumpkin, Inc. All Rights Reserved.

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\CubeSatKit\\PIC24\\Inc\\csk_uart.h,v $
$Author: aek $
$Revision: 3.6 $
$Date: 2014-06-02 09:04:50-07 $

******************************************************************************/
#ifndef __csk_uart_h
#define __csk_uart_h


// Microchip's peripheral library functions
#include <uart.h>

// Pumpkin PIC24 library functions
#include "pic24_uart.h"
#include "pic24_uart_msg.h"
#include "pic24_uart_msg_ts.h"
#include "pic24_uart_msg_hhmmsstt.h"

// Aliases for commonly-used functions that are pulled from the
//  Microchip PIC24F Peripheral Library
#define CSK_UART_9600_N81              UART_9600_N81_MAIN
#define CSK_UART_38400_N81             UART_38400_N81_MAIN
#define CSK_UART_57600_N81             UART_57600_N81_MAIN
#define CSK_UART_115200_N81            UART_115200_N81_MAIN

// CSK UART0 <==> PIC24 UART1
#define csk_uart0_close()              uart1_close()
#define csk_uart0_inchar(x)            uart1_inchar(x)
#define csk_uart0_getchar()            uart1_getchar()
#define csk_uart0_open(x)              uart1_open(x)
#define csk_uart0_msg(x)               uart1_msg(x)
#define csk_uart0_msg_ts(x)            uart1_msg_ts(x)
#define csk_uart0_msg_hhmmsstt(x)      uart1_msg_hhmmsstt(x)
#define csk_uart0_outchar()            uart1_outchar()
#define csk_uart0_putchar(x)           uart1_putchar(x)
#define csk_uart0_puts(x)              uart1_puts(x)
#define csk_uart0_rx_count()					 uart1_rx_count()

// CSK UART1 <==> PIC24 UART2
#define csk_uart1_close()              uart2_close()
#define csk_uart1_inchar(x)            uart2_inchar(x)
#define csk_uart1_getchar()            uart2_getchar()
#define csk_uart1_open(x)              uart2_open(x)
#define csk_uart1_msg(x)               uart2_msg(x)
#define csk_uart1_msg_ts(x)            uart2_msg_ts(x)
#define csk_uart1_msg_hhmmsstt(x)      uart2_msg_hhmmsstt(x)
#define csk_uart1_outchar()            uart2_outchar()
#define csk_uart1_putchar(x)           uart2_putchar(x)
#define csk_uart1_puts(x)              uart2_puts(x)
#define csk_uart1_rx_count()					 uart2_rx_count()

// CSK UART2 <==> PIC24 UART3
#define csk_uart2_close()              uart3_close()
#define csk_uart2_inchar(x)            uart3_inchar(x)
#define csk_uart2_getchar()            uart3_getchar()
#define csk_uart2_open(x)              uart3_open(x)
#define csk_uart2_msg(x)               uart3_msg(x)
#define csk_uart2_msg_ts(x)            uart3_msg_ts(x)
#define csk_uart2_msg_hhmmsstt(x)      uart3_msg_hhmmsstt(x)
#define csk_uart2_outchar()            uart3_outchar()
#define csk_uart2_putchar(x)           uart3_putchar(x)
#define csk_uart2_puts(x)              uart3_puts(x)
#define csk_uart2_rx_count()					 uart3_rx_count()

// CSK UART3 <==> PIC24 UART4
#define csk_uart3_close()              uart4_close()
#define csk_uart3_inchar(x)            uart4_inchar(x)
#define csk_uart3_getchar()            uart4_getchar()
#define csk_uart3_open(x)              uart4_open(x)
#define csk_uart3_msg(x)               uart4_msg(x)
#define csk_uart3_msg_ts(x)            uart4_msg_ts(x)
#define csk_uart3_msg_hhmmsstt(x)      uart4_msg_hhmmsstt(x)
#define csk_uart3_outchar()            uart4_outchar()
#define csk_uart3_putchar(x)           uart4_putchar(x)
#define csk_uart3_puts(x)              uart4_puts(x)
#define csk_uart3_rx_count()					 uart4_rx_count()


#endif /* __csk_uart_h */
