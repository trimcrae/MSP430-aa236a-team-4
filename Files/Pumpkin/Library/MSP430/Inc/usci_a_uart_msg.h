/************************************************************
Copyright (C) 1995-2007 Pumpkin, Inc. and its
Licensor(s).

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\Library\\MSP430\\Inc\\usci_a_uart_msg.h,v $
$Author: aek $
$Revision: 3.0 $
$Date: 2009-11-01 22:53:31-08 $

************************************************************/

#ifndef __usci_a_uart_msg_h
#define __usci_a_uart_msg_h


#ifndef GETCHAR_PUTCHAR_RETURN_ZERO
#define  EOF_RETURN_CODE            EOF
#else
#define  EOF_RETURN_CODE            0
#endif

#define CRLF                                "\n\r"
#define TICKS_BUFFER_SIZE                   15

extern void usci_a0_uart_msg(const char *);
extern void usci_a1_uart_msg(const char *);


#endif /* __usci_a_uart_msg_h */


