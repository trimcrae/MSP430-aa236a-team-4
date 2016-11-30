/******************************************************************************
(C) Copyright Pumpkin, Inc. All Rights Reserved.

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\Library\\PIC24\\Inc\\pic24_i2c.h,v $
$Author: aek $
$Revision: 3.0 $
$Date: 2011-09-17 23:42:56-07 $

******************************************************************************/
#ifndef __pic24_i2c1_h
#define __pic24_i2c1_h


// Function prototypes.
extern void i2c1_open(void);
extern int  i2c1_write(unsigned char, unsigned char *, unsigned int);
extern int  i2c1_read(unsigned char, unsigned char *, unsigned int);


#endif /* __i2c1_h */
