/************************************************************
Copyright (C) 1995-2007 Pumpkin, Inc. and its
Licensor(s). 

This file may be distributed under the terms of the License 
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, 
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND 
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\Salvo\\Example\\MSP430\\MSP430x1xx\\TI\040MSP-TS430PM64\\Tut\\Tut5\\tut5.h,v $
$Author: aek $
$Revision: 3.2 $
$Date: 2007-11-29 11:48:55-08 $

Header file for tutorial program Tut5.

************************************************************/

#define DCO_CLOCK_FREQ          760000L     // assume 760kHz DCO clock
#define TICK_RATE               100         // OS timer tick rate, in Hz

#define TIMERA0_RELOAD          DCO_CLOCK_FREQ/TICK_RATE
