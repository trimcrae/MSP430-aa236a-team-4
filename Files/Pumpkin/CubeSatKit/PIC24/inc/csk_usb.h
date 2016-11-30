/******************************************************************************
(C) Copyright Pumpkin, Inc. All Rights Reserved.

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\CubeSatKit\\PIC24\\Inc\\csk_usb.h,v $
$Author: aek $
$Revision: 3.0 $
$Date: 2010-01-24 10:25:07-08 $

******************************************************************************/
#ifndef __csk_usb_h
#define __csk_usb_h


extern void csk_usb_open(void);
extern void csk_usb_close(void);
unsigned char csk_usb_present(void);


#endif /* __csk_usb_h */