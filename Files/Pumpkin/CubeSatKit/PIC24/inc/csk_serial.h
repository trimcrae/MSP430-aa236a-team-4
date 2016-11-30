/******************************************************************************
(C) Copyright Pumpkin, Inc. All Rights Reserved.

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\CubeSatKit\\PIC24\\Inc\\csk_serial.h,v $
$Author: aek $
$Revision: 3.1 $
$Date: 2011-09-17 16:24:48-07 $

******************************************************************************/
#ifndef __csk_serial_h
#define __csk_serial_h


extern int write(int, void *, unsigned int);
extern void csk_serial_stdin_select(unsigned char);
extern unsigned char csk_serial_get_byte(unsigned char, unsigned char *);


#endif /* __csk_serial_h */
