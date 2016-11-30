/******************************************************************************
(C) Copyright Pumpkin, Inc. All Rights Reserved.

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\CubeSatKit\\PIC24\\Inc\\csk_i2c.h,v $
$Author: aek $
$Revision: 3.0 $
$Date: 2011-09-17 15:49:15-07 $

******************************************************************************/
#ifndef __csk_i2c_h
#define __csk_i2c_h


// Microchip's peripheral library functions
#include <i2c.h>

// Pumpkin PIC24 library functions
#include "pic24_i2c.h"

// CSK I2C0 <==> PIC24 I2C1
#define csk_i2c0_open                  i2c1_open
#define csk_i2c0_read                  i2c1_read
#define csk_i2c0_write                 i2c1_write


#endif /* __csk_i2c_h */
