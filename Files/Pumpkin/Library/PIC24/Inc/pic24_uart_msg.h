/************************************************************
Copyright (C) 1995-2007 Pumpkin, Inc. and its
Licensor(s).

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\Library\\PIC24\\Inc\\pic24_uart_msg.h,v $
$Author: aek $
$Revision: 3.1 $
$Date: 2013-11-08 22:14:12-08 $

************************************************************/

#ifndef __pic24_uart_msg_h
#define __pic24_uart_msg_h


#ifndef GETCHAR_PUTCHAR_RETURN_ZERO
#define  EOF_RETURN_CODE            EOF
#else
#define  EOF_RETURN_CODE            0
#endif

#define CRLF                                "\r\n"
#define TICKS_BUFFER_SIZE                   15

extern void uart1_msg(const char *);
extern void uart2_msg(const char *);
extern void uart3_msg(const char *);
extern void uart4_msg(const char *);


#endif /* __pic24_uart_msg_h */


