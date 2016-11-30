/************************************************************
Copyright (C) 1995-2007 Pumpkin, Inc. and its
Licensor(s).

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\Library\\MSP430\\Inc\\usci_a_uart.h,v $
$Author: aek $
$Revision: 3.0 $
$Date: 2009-11-01 22:53:31-08 $

************************************************************/

#ifndef __usci_a_uart_h
#define __usci_a_uart_h

#include "config.h"

// For PZ packages only ...
#if   defined(__MSP430FG4616__) || (__TARGET_PROCESSOR==MSP430FG4616) \
   || defined(__MSP430FG4617__) || (__TARGET_PROCESSOR==MSP430FG4617) \
   || defined(__MSP430FG4618__) || (__TARGET_PROCESSOR==MSP430FG4618) \
   || defined(__MSP430FG4619__) || (__TARGET_PROCESSOR==MSP430FG4619)
#define usci_a0_uart_open_io()     do { IO_LOW(P2,BIT5); \
                                        IO_INPUT(P2,BIT5); \
                                        IO_HIGH(P2,BIT4); \
                                        IO_OUTPUT(P2,BIT4); \
                                        IO_SEL(P2,BIT5+BIT4); } while (0)

#define usci_a0_uart_close_io()    do { IO_HIGH(P2,BIT5); \
                                        IO_UNSEL(P2,BIT5+BIT4); } while (0)


// For PM packages only ...
#elif defined(__MSP430F2416__) || (__TARGET_PROCESSOR==MSP430F2416) \
   || defined(__MSP430F2417__) || (__TARGET_PROCESSOR==MSP430F2417) \
   || defined(__MSP430F2418__) || (__TARGET_PROCESSOR==MSP430F2418) \
   || defined(__MSP430F2419__) || (__TARGET_PROCESSOR==MSP430F2419) \
   || defined(__MSP430F2616__) || (__TARGET_PROCESSOR==MSP430F2616) \
   || defined(__MSP430F2617__) || (__TARGET_PROCESSOR==MSP430F2617) \
   || defined(__MSP430F2618__) || (__TARGET_PROCESSOR==MSP430F2618) \
   || defined(__MSP430F2619__) || (__TARGET_PROCESSOR==MSP430F2619) 
#define usci_a0_uart_open_io()     do { IO_LOW(P3,BIT5); \
                                        IO_INPUT(P3,BIT5); \
                                        IO_HIGH(P3,BIT4); \
                                        IO_OUTPUT(P3,BIT4); \
                                        IO_SEL(P3,BIT5+BIT4); } while (0)

#define usci_a0_uart_close_io()    do { IO_HIGH(P3,BIT5); \
                                        IO_UNSEL(P3,BIT5+BIT4); } while (0)

#define usci_a1_uart_open_io()     do { IO_LOW(P3,BIT7); \
                                        IO_INPUT(P3,BIT7); \
                                        IO_HIGH(P3,BIT6); \
                                        IO_OUTPUT(P3,BIT6); \
                                        IO_SEL(P3,BIT7+BIT6); } while (0)

#define usci_a1_uart_close_io()    do { IO_HIGH(P3,BIT7); \
                                        IO_UNSEL(P3,BIT7+BIT6); } while (0)



#else
#error  usci_a_uart.h requires port pin definitions for USCI A0/A1 UART.

#endif  /* defined (__MSP430FG4616__) || ... */




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
#define RX0_BUFF_SIZE             64
#endif

#ifndef TX0_BUFF_SIZE
#define TX0_BUFF_SIZE             64
#endif

#ifndef RX1_BUFF_SIZE
#define RX1_BUFF_SIZE             64
#endif

#ifndef TX1_BUFF_SIZE
#define TX1_BUFF_SIZE             64
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
// Settings commented out have not yet had their UCBRSx values calculated ...
//#define USCI_A_UART_300_N81_ACLK                             (        UCSSEL0) , 0x0000, 0x006C // needs modulation / correction
//#define USCI_A_UART_600_N81_ACLK                             (        UCSSEL0) , 0x0000, 0x0036 // needs modulation / correction
#define USCI_A_UART_1200_N81_ACLK                             (        UCSSEL0) ,   2<<1, 27
#define USCI_A_UART_2400_N81_ACLK                             (        UCSSEL0) ,   6<<1, 13
#define USCI_A_UART_4800_N81_ACLK                             (        UCSSEL0) ,   7<<1, 6
#define USCI_A_UART_9600_N81_ACLK                             (        UCSSEL0) ,   3<<1, 3
#define USCI_A_UART_9600_N82_ACLK         (((UCSPB      )<<8)+(        UCSSEL0)),   3<<1, 3
//#define USCI_A_UART_300_N81_SMCLK                             (UCSSEL1+UCSSEL0) , 0x0000, (SMCLK/   300)
//#define USCI_A_UART_600_N81_SMCLK                             (UCSSEL1+UCSSEL0) , 0x0000, (SMCLK/   600)
//#define USCI_A_UART_1200_N81_SMCLK                            (UCSSEL1+UCSSEL0) , 0x0000, (SMCLK/  1200)
//#define USCI_A_UART_2400_N81_SMCLK                            (UCSSEL1+UCSSEL0) , 0x0000, (SMCLK/  2400)
//#define USCI_A_UART_4800_N81_SMCLK                            (UCSSEL1+UCSSEL0) , 0x0000, (SMCLK/  4800)
//#define USCI_A_UART_4800_O81_SMCLK     (((UCPEN      )<<8)+(UCSSEL1+UCSSEL0)), 0x0000, (SMCLK/  4800)
//#define USCI_A_UART_9600_E81_SMCLK     (((UCPEN+UCPAR)<<8)+(UCSSEL1+UCSSEL0)), 0x0000, (SMCLK/  9600)
#define USCI_A_UART_9600_N81_SMCLK                            (UCSSEL1+UCSSEL0) , 4<<1, (SMCLK/  9600) // Needs UCBRS definition -- guessing at 4
//#define USCI_A_UART_9600_O81_SMCLK     (((UCPEN      )<<8)+(UCSSEL1+UCSSEL0)), 0x0000, (SMCLK/  9600)
//#define USCI_A_UART_19200_N81_SMCLK                           (UCSSEL1+UCSSEL0) , 0x0000, (SMCLK/ 19200)
//#define USCI_A_UART_38400_N81_SMCLK                           (UCSSEL1+UCSSEL0) , 0x0000, (SMCLK/ 38400)
#define USCI_A_UART_57600_N81_SMCLK                           (UCSSEL1+UCSSEL0) , 7<<1, (SMCLK/ 57600)
//#define USCI_A_UART_76800_N81_SMCLK                           (UCSSEL1+UCSSEL0) , 0x0000, (SMCLK/ 76800)
//#define USCI_A_UART_115200_N81_SMCLK                          (UCSSEL1+UCSSEL0) , 0x0000, (SMCLK/115200)


// Function prototypes.
extern void          usci_a0_uart_close(void);
extern int           usci_a0_uart_getchar(void);
extern void          usci_a0_uart_inchar(unsigned char);
extern void          usci_a0_uart_open(unsigned int, unsigned int, unsigned int);
extern void          usci_a0_uart_outchar(void);
extern int           usci_a0_uart_peek_rx(unsigned int);
extern int           usci_a0_uart_putchar(int);
extern int           usci_a0_uart_puts(const char *);
extern unsigned int  usci_a0_uart_rx_count(void);
extern unsigned int  usci_a0_uart_tx_free(void);

extern void          usci_a1_uart_close(void);
extern int           usci_a1_uart_getchar(void);
extern void          usci_a1_uart_inchar(unsigned char);
extern void          usci_a1_uart_open(unsigned int, unsigned int, unsigned int);
extern void          usci_a1_uart_outchar(void);
extern int           usci_a1_uart_peek_rx(unsigned int);
extern int           usci_a1_uart_putchar(int);
extern int           usci_a1_uart_puts(const char *);
extern unsigned int  usci_a1_uart_rx_count(void);
extern unsigned int  usci_a1_uart_tx_free(void);


#endif /* __usci_a_uart_h */


